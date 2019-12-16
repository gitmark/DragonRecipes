/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include <string>
#include <memory>

#include <DragonRecipes/Common.h>

namespace dragon {

class LogListenerPrivate;

class DRAGON_EXPORT LogListener {
  public:
      LogListener(std::function<void(const std::string&)> lambda);
    LogListener(const LogListener &logListener) = delete;
    LogListener(LogListener &&logListener) noexcept = delete;
    LogListener &operator=(const LogListener &logListener) = delete;
    LogListener &operator=(LogListener &&logListener) noexcept = delete;

    virtual ~LogListener();

    virtual void write(const std::string &msg);

  private:
    std::unique_ptr<LogListenerPrivate> data;
};

using LogListenerPtr = std::shared_ptr<LogListener>;

inline LogListenerPtr newLogListener(std::function<void(const std::string&)> lambda) {
    return std::make_shared<LogListener>(lambda);
}

class LogWriterPrivate;

class DRAGON_EXPORT LogWriter {
  public:
    LogWriter();

    LogWriter(LogWriter &&logBuf) noexcept = delete;
    LogWriter(LogWriter &logBuf) = delete;
    LogWriter &operator=(const LogWriter &logBuf) = delete;
    LogWriter &operator=(LogWriter &&logBuf) noexcept = delete;
    virtual ~LogWriter();

    void addListener(const LogListenerPtr &listener);
    void setListener(const LogListenerPtr &listener);
    void write(const char *buf, int count);

  private:
    std::unique_ptr<LogWriterPrivate> data;
};

using LogWriterPtr = std::shared_ptr<LogWriter>;

inline LogWriterPtr newLogWriter() {
    return std::make_shared<LogWriter>();
}
}

#endif
