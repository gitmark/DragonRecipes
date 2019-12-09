/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef LOG_H
#define LOG_H

#include <DragonRecipes/LogBuf.h>
#include <DragonRecipes/LogWriter.h>

constexpr int STREAM_BUF_SIZE = 10000;
// References:
// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
// http://stackoverflow.com/questions/21420526/implementing-stdbasic-streambuf-subclass-for-manipulating-input

namespace dragon {

// B = std::iostream
// T = LogBuf
// A = ctor input param
template<class B, class T, class A>
class LogStream_ : public B {
  public:
    explicit LogStream_(A *logWriter = nullptr);
    explicit LogStream_(std::unique_ptr<A> &logWriter);
    void addListener(LogListenerPtr listener) {
        logWriter->addListener(listener);
    }
    void setListener(LogListenerPtr listener) {
        logWriter->setListener(listener);
    }
    int error();
    void clearError();

  private:
    T sb;
    A *logWriter;
};

template<class B, class T, class A>
LogStream_<B,T,A>::LogStream_(A *logWriter)
    : std::ios(nullptr), B(&sb), sb(logWriter,STREAM_BUF_SIZE), logWriter(logWriter)
{}

template<class B, class T, class A>
LogStream_<B,T,A>::LogStream_(std::unique_ptr<A> &logWriter)
    : std::ios(0), B(&sb), sb(logWriter.get(),STREAM_BUF_SIZE), logWriter(logWriter.get())
{}

template<class B, class T, class A>
int LogStream_<B,T,A>::error() {
    return sb.error();
}

template<class B, class T, class A>
void LogStream_<B,T,A>::clearError() {
    return sb.clearError();
}

using LogStream = LogStream_<std::iostream, LogBuf<LogWriter, LogWriter,
        AssignBuf<LogWriter,LogWriter>>, LogWriter>;

namespace log {
DRAGON_EXPORT extern LogWriter debugLogWriter;
DRAGON_EXPORT extern LogStream debug;
DRAGON_EXPORT extern LogWriter infoLogWriter;
DRAGON_EXPORT extern LogStream info;
DRAGON_EXPORT extern LogWriter warnLogWriter;
DRAGON_EXPORT extern LogStream warn;
DRAGON_EXPORT extern LogWriter errorLogWriter;
DRAGON_EXPORT extern LogStream error;

}

}

#endif

