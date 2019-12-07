/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include <string>
#include <PrivateHeaders/Version.h>
#include <DragonRecipes/Version.h>

#include <iostream>

namespace dragon {

std::string dragonRecipesVersion() {
    return std::string(DRAGON_RECIPES_VERSION) + " " + DRAGON_RECIPES_DEV_STAGE;
}

}
