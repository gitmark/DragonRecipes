/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include <string>
#include <memory>

#include <DragonRecipes/Constants.h>

namespace dragon {

class LogListenerPrivate;

class DRAGON_EXPORT LogListener {
  public:
    LogListener(std::function<void(const std::string&)> lambda);
    virtual ~LogListener();

    virtual void write(const std::string &msg);

  protected:
    std::unique_ptr<LogListenerPrivate> data;
};

typedef std::shared_ptr<LogListener> LogListenerPtr;

inline LogListenerPtr newLogListener(std::function<void(const std::string&)> lambda) {
    return std::make_shared<LogListener>(lambda);
}

class LogWriterPrivate;

class DRAGON_EXPORT LogWriter {
  public:
    LogWriter();
    virtual ~LogWriter();

    void addListener(LogListenerPtr listener);
    void setListener(LogListenerPtr listener);
    void write(const char *buf, int count);

  protected:
    std::unique_ptr<LogWriterPrivate> data;
};

typedef std::shared_ptr<LogWriter> LogWriterPtr;

inline LogWriterPtr newLogWriter() {
    return std::make_shared<LogWriter>();
}
}

#endif
