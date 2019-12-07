/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

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

