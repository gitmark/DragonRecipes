/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef SYMBOL_PRIVATE_H
#define SYMBOL_PRIVATE_H

#include <string>

#include <DragonRecipes/Symbol.h>

#define SYMBOL_TYPE_COUNT 3

namespace dragon
    {
    class SymbolPrivate
    {
    public:
        SymbolPrivate(const std::string &name = "", int id = 0, Symbol::Type type = Symbol::unknown) :
            name(name), id(id), type(type) {}
        virtual ~SymbolPrivate();

        std::string name;
        int id;
        Symbol::Type type;
        static const char *typeStrings[SYMBOL_TYPE_COUNT];
    };

}

#endif
