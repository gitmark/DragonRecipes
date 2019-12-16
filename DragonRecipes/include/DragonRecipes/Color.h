/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef COLOR_H
#define COLOR_H

#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Color {
public:
    Color(int red = 0, int green = 0, int blue = 0, int alpha = 255) : red(red), green(green), blue(blue), alpha(alpha) {}

    int red;
    int green;
    int blue;
    int alpha;
};

}

#endif
