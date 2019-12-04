/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Log.h>

namespace dragon
    {

std::vector<LogWriterPtr> initLogs() {
    std::vector<LogWriterPtr> Logs;
    Logs.push_back(newLogWriter());
    return Logs;
}

std::vector<LogWriterPtr> &LogWriters() {
    static std::vector<LogWriterPtr> _LogWriters = initLogs();
    return _LogWriters;
}

class LogListenerPrivate
{
public:
    LogListenerPrivate(std::function<void(const std::string&)> lambda) : lambda(lambda) {}
    virtual ~LogListenerPrivate();

    std::function<void(const std::string&)> lambda;
};

LogListenerPrivate::~LogListenerPrivate() {}

LogListener::LogListener(std::function<void(const std::string&)> lambda) : data(new LogListenerPrivate(lambda)) {
 }

LogListener::~LogListener() {
}

void LogListener::write(const std::string &msg) {
    data->lambda(msg);
}

    class LogWriterPrivate
    {
    public:
        LogWriterPrivate() {}
        virtual ~LogWriterPrivate();

        std::vector<LogListenerPtr> Listeners;
    };

    LogWriterPrivate::~LogWriterPrivate() {}


    LogWriter::LogWriter() : data(new LogWriterPrivate()) {}
    
    LogWriter::~LogWriter() {
    }

    void LogWriter::addListener(LogListenerPtr listener) {
        data->Listeners.push_back(listener);
    }

    void LogWriter::setListener(LogListenerPtr listener) {
        data->Listeners.clear();
        data->Listeners.push_back(listener);
    }

    void LogWriter::write(const char *buf, int count) {
        std::string msg(buf, static_cast<size_t>(count));
        for (auto Listener : data->Listeners)
            Listener->write(msg);
    }

    }
