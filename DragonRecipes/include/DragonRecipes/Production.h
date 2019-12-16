/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>
#include <string>
#include <memory>
#include <DragonRecipes/Common.h>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/Token.h>

namespace dragon {

class ProductionPrivate;

class DRAGON_EXPORT Production {
  public:
    Production(const std::string &head, const std::string &body, std::function<int(std::vector<NodePtr>&,TokenPtr)> action = [](std::vector<NodePtr>&,TokenPtr){return 0;});
    Production(const Production&) = delete;
    Production(Production&&) = delete;
    Production &operator=(const Production &production) = delete;
    Production &operator=(Production &&production) noexcept = delete;

    virtual ~Production();

    std::vector<std::string> bodyVec();
    std::string body();
    std::string head();
    void print(std::ostream &os);
    std::string toString();
    int action(std::vector<NodePtr>&,TokenPtr);

  private:
      std::unique_ptr<ProductionPrivate> data;
};

using ProdPtr = std::shared_ptr<Production>;

inline ProdPtr newProduction(const std::string &head, const std::string &body, std::function<int(std::vector<NodePtr>&,TokenPtr)> action = [](std::vector<NodePtr>&,TokenPtr){return 0;}) {
    return std::make_shared<Production>(head, body, action);
}
}


#endif
