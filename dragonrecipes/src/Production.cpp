/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include "dragonrecipes/Production.h"
#include "dragonrecipes/StringTools.h"

namespace dr
    {
    class ProductionPrivate
    {
    public:
        ProductionPrivate(const std::string &head, const std::string &body) :
        head(head) {
            split(body, " ", this->body);
        }
        
        std::string head;
        std::vector<std::string> body;
    };
    
    Production::Production(const std::string &head, const std::string &body) :
    data(new ProductionPrivate(head, body)) {}
    
    Production::~Production() {
    }
    
    std::vector<std::string> Production::body() {
        return data->body;
    }

    std::string Production::bodyString() {
        return join(data->body, " ");
    }

    std::string Production::head() {
        return data->head;
    }
    
    void Production::print(std::ostream &os)
    {
        os << data->head << " -> " << bodyString();
    }
    
    std::string Production::toString() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }
    
    
    
    }
