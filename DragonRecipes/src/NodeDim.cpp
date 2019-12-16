/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include <DragonRecipes/NodeDim.h>

namespace dragon {

NodeDim::~NodeDim() {
    _children.clear();
}

}
