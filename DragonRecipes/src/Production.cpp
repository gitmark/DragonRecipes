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
    ProductionPrivate(std::string head, const std::string &body) :
          head(std::move(head)) {
        split(body, " ", this->body);
    }

private:
    std::string head;
    std::vector<std::string> body;
    friend class Production;
};

Production::Production(const std::string &head, const std::string &body) :
    data(new ProductionPrivate(head, body)) {}

Production::~Production() {
    data.reset(nullptr);
}

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
