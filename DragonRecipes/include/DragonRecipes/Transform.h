/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Common.h>

namespace dragon {


inline int toInt(double d) {
    return static_cast<int>(std::rint(d));
}

inline int toInt(float f) {
    return static_cast<int>(std::rint(f));
}


inline Point rotate(Point center, float angle, Point point) {

    float x = 0;
    float y = 0;

    float x1 = point.x - center.x;
    float y1 = point.y - center.y;

    float c = std::cos(-angle);
    float s = std::sin(-angle);
    x = c*x1 - s*y1;
    y = s*x1 + c*y1;

    return Point(center.x + x, center.y + y);
}

inline Point scale(Point center, float s, Point point) {

    float x = 0;
    float y = 0;

    float x1 = point.x - center.x;
    float y1 = point.y - center.y;

    x = s*x1;
    y = s*y1;

    return Point(center.x + x, center.y + y);
}

}

#endif
