/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef BRUSH_H
#define BRUSH_H

#include <DragonRecipes/Color.h>
#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Brush {
public:
    Brush(Color color) : color(color) {}

    Color color;
};

}

#endif
