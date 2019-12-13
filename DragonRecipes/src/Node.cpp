/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/
#include <functional>
#include <utility>
#include <iostream>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Symbol.h>

namespace dragon {

class NodePrivate {
  public:
    NodePrivate(SymbolPtr symbol) :
          symbol(symbol) {
    }

private:
    SymbolPtr symbol;
    std::vector<NodePtr> children;

    friend class Node;
};

Node::Node(SymbolPtr symbol) :
    data(new NodePrivate(symbol)) {}

Node::~Node() {
    data.reset(nullptr);
}

std::vector<NodePtr> &Node::children() {
    return data->children;
}

SymbolPtr &Node::symbol() {
    return data->symbol;
}

void Node::addChild(NodePtr child) {
    data->children.push_back(child);
}

void Node::print(std::ostream &os) {
    data->symbol->print(os);
}

std::string Node::toString() {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

}
