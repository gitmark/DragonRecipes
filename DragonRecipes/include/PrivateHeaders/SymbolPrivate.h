/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef SYMBOL_PRIVATE_H
#define SYMBOL_PRIVATE_H

#include <string>
#include <vector>

#include <DragonRecipes/Symbol.h>

namespace dragon {
class Symbol;
class Token;

class SymbolPrivate {
  public:
      SymbolPrivate(std::string name = std::string(), int id = 0, Symbol::Type type = Symbol::unknown) :
          name(std::move(name)), id(id), type(type) {}

      SymbolPrivate(const SymbolPrivate &symbolPrivate) = default;
      SymbolPrivate(SymbolPrivate &&symbolPrivate) noexcept = default;
      SymbolPrivate &operator=(const SymbolPrivate &symbolPrivate) = default;
      SymbolPrivate &operator=(SymbolPrivate &&symbolPrivate) noexcept = default;
      virtual ~SymbolPrivate() = default;


    static std::string at(size_t index) {
        return typeStrings[index];

    }

private:
    std::string name;
    int id;
    Symbol::Type type;
    static std::vector<std::string> typeStrings;

    friend Symbol;
    friend Token;
};

}

#endif
