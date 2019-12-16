/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef CELL_H
#define CELL_H

#include <string>
#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Cell {
public:
    Cell(const std::string &str = "", int usage = 0)
        : str(str), usage(usage) {}

    std::string str;
    int usage;
};

}

#endif
