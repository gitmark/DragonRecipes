/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef RECT_H
#define RECT_H

#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Rect {
public:
    Rect(float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    float x1;
    float y1;
    float x2;
    float y2;
};

}

#endif

