/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <DragonRecipes/Symbol.h>
#include <PrivateHeaders/SymbolPrivate.h>
#include <DragonRecipes/StringTools.h>

namespace dragon {


Symbol::Symbol(const std::string &name, int id, Type type) :
    data(new SymbolPrivate(name, id, type)) {}

Symbol::Symbol(std::unique_ptr<SymbolPrivate> &&ptr) :
    data(std::move(ptr)) {}

Symbol::Symbol(const Symbol &symbol) :
    data(new SymbolPrivate()) {
    *data = *symbol.data;
}

Symbol::Symbol(Symbol &&symbol) noexcept :
    data(new SymbolPrivate()) {
    std::swap(data, symbol.data);
}

Symbol &Symbol::operator=(const Symbol &symbol) {
    *data = *symbol.data;
    return *this;
}

Symbol &Symbol::operator=(Symbol &&symbol) noexcept {
    std::swap(data, symbol.data);
    return *this;
}

Symbol::~Symbol() {
    data.reset(nullptr);
}

void Symbol::print(std::ostream &os) const {
    os << "class: Symbol\n";
    os << "name: " << data->name << "\n";
    os << "id: " << data->id << "\n";
    os << "type: " << typeStr() << "\n";
}

std::string Symbol::toString() const {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

int Symbol::id() const {
    return data->id;
}

std::string Symbol::name() const {
    return data->name;
}

Symbol::Type Symbol::type() const {
    return data->type;
}

std::string Symbol::typeStr() const {
    return SymbolPrivate::at(data->type);
}


void Symbol::setId(int id) {
    data->id = id;
}
void Symbol::setName(const std::string &name) {
    data->name = name;
}

void Symbol::setType(Symbol::Type type) {
    data->type = type;
}

}
