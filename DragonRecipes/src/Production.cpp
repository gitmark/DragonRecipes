/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/
#include <functional>
#include <utility>
#include <iostream>
#include "DragonRecipes/Production.h"
#include "DragonRecipes/StringTools.h"
#include "DragonRecipes/Node.h"
#include "DragonRecipes/Token.h"

namespace dragon {

class ProductionPrivate {
  public:
    ProductionPrivate(std::string head, const std::string &body, std::function<int(std::vector<NodePtr>&,TokenPtr)> action) :
          head(std::move(head)), action(action) {
        split(body, " ", this->body);
    }

private:
    std::string head;
    std::vector<std::string> body;
    std::function<int(std::vector<NodePtr>&,TokenPtr)> action;
    friend class Production;
};

Production::Production(const std::string &head, const std::string &body, std::function<int(std::vector<NodePtr>&,TokenPtr)> action) :
    data(new ProductionPrivate(head, body, action)) {}

Production::~Production() {
    data.reset(nullptr);
}

int Production::action(std::vector<NodePtr> &node, TokenPtr token) {
    return data->action(node,token);
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
