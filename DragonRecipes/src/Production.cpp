/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include "DragonRecipes/Production.h"
#include "DragonRecipes/StringTools.h"

namespace dragon {

class ProductionPrivate {
  public:
    ProductionPrivate(const std::string &head, const std::string &body) :
        head(head) {
        split(body, " ", this->body);
    }

    std::string head;
    std::vector<std::string> body;
};

UNIQUE_PTR_IMPL(ProductionPrivate)

Production::Production(const std::string &head, const std::string &body) :
    data(new ProductionPrivate(head, body)) {}

Production::~Production() {}

std::vector<std::string> Production::bodyVec() {
    return data->body;
}

std::string Production::body() {
    return join(data->body, " ");
}

std::string Production::head() {
    return data->head;
}

void Production::print(std::ostream &os) {
    os << data->head << " -> " << body();
}

std::string Production::toString() {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

}
