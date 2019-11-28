/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <vector>
#include <string>
#include <memory>

#include "dragonrecipes/Constants.h"

namespace dr
{
class ProductionPrivate;

    class DRAGON_EXPORT Production
    {
    public:
        Production(const std::string &head, const std::string &body);
        Production(const Production&) = delete;
        Production(Production&&) = delete;
        virtual ~Production();
        
        std::vector<std::string> body();
        std::string bodyString();
        std::string head();
        void print(std::ostream &os);
        std::string toString();
 
    protected:
        std::unique_ptr<ProductionPrivate> data;
    };
}

#endif
