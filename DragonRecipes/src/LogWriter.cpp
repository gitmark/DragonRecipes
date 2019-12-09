/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Log.h>

namespace dragon {

std::vector<LogWriterPtr> initLogs() {
    std::vector<LogWriterPtr> Logs;
    Logs.push_back(newLogWriter());
    return Logs;
}

std::vector<LogWriterPtr> &LogWriters() {
    static std::vector<LogWriterPtr> _LogWriters = initLogs();
    return _LogWriters;
}

class LogListenerPrivate {
  public:
      LogListenerPrivate(std::function<void(const std::string&)> lambda) : lambda(std::move(lambda)) {}
    LogListenerPrivate(const LogListenerPrivate &logListenerPrivate) = default;
    LogListenerPrivate(LogListenerPrivate &&Error) noexcept = default;
    LogListenerPrivate &operator=(const LogListenerPrivate &Error) = default;
    LogListenerPrivate &operator=(LogListenerPrivate &&Error) noexcept = default;
    virtual ~LogListenerPrivate() = default;

private:
    std::function<void(const std::string&)> lambda;

    friend LogListener;
};

LogListener::LogListener(std::function<void(const std::string&)> lambda) : data(new LogListenerPrivate(std::move(lambda))) {
}

LogListener::~LogListener() {
    data.reset(nullptr);
}
void LogListener::write(const std::string &msg) {
    data->lambda(msg);
}

class LogWriterPrivate {
  public:
      LogWriterPrivate() = default;
      LogWriterPrivate(const LogWriterPrivate &logWriterPrivate) = default;
      LogWriterPrivate(LogWriterPrivate &&logWriterPrivate) noexcept = default;
      LogWriterPrivate &operator=(const LogWriterPrivate &logWriterPrivate) = default;
      LogWriterPrivate &operator=(LogWriterPrivate &&logWriterPrivate) noexcept = default;
    virtual ~LogWriterPrivate() = default;

private:
    std::vector<LogListenerPtr> Listeners;
    friend LogWriter;
};

LogWriter::LogWriter() : data(new LogWriterPrivate()) {}

LogWriter::~LogWriter() {
    data.reset(nullptr);
}

void LogWriter::addListener(const LogListenerPtr &listener) {
    data->Listeners.push_back(listener);
}

void LogWriter::setListener(const LogListenerPtr &listener) {
    data->Listeners.clear();
    data->Listeners.push_back(listener);
}

void LogWriter::write(const char *buf, int count) {
    std::string msg(buf, static_cast<size_t>(count));
    for (const auto &Listener : data->Listeners)
        Listener->write(msg);
}

}
