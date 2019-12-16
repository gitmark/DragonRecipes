/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef POINT_H
#define POINT_H

#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Point {
public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    float x;
    float y;
};

}

#endif
