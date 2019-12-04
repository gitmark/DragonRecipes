#include <DragonRecipes/Log.h>

namespace dragon {

namespace log {
    LogWriter debugLogWriter;
    LogStream debug(&infoLogWriter);
    LogWriter infoLogWriter;
    LogStream info(&infoLogWriter);
    LogWriter warnLogWriter;
    LogStream warn(&infoLogWriter);
    LogWriter errorLogWriter;
    LogStream error(&infoLogWriter);
}

}

