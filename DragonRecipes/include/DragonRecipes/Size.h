/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef SIZE_H
#define SIZE_H

#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Size {
public:
    Size(float width = 0, float height = 0) : width(width), height(height) {}
    float width;
    float height;
};

}

#endif
