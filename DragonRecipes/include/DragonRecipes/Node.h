/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <memory>
#include <DragonRecipes/Common.h>
#include <DragonRecipes/Symbol.h>

namespace dragon {

class NodePrivate;
class Node;
using NodePtr = std::shared_ptr<Node>;

class DRAGON_EXPORT Node {
  public:
    Node(SymbolPtr symbol);
    Node(const Node&) = delete;
    Node(Node&&) = delete;
    Node &operator=(const Node &Node) = delete;
    Node &operator=(Node &&Node) noexcept = delete;

    virtual ~Node();

    std::vector<NodePtr> &children();

    SymbolPtr &symbol();

    void addChild(NodePtr child);
    void print(std::ostream &os);
    std::string toString();

  private:
      std::unique_ptr<NodePrivate> data;
};


inline NodePtr newNode(SymbolPtr symbol) {
    return std::make_shared<Node>(symbol);
}
}


#endif
