/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <PrivateHeaders/SymbolPrivate.h>
#include <DragonRecipes/StringTools.h>


namespace dragon
    {

    const char *SymbolPrivate::typeStrings[SYMBOL_TYPE_COUNT] = { "unknown", "terminal", "nonterminal" };

    }
