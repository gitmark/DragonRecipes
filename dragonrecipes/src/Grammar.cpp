/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <set>

#include <dragonrecipes/Grammar.h>
#include <dragonrecipes/StringTools.h>
#include <dragonrecipes/Production.h>

namespace dr
    {

    class GrammarPrivate
    {
    public:
        GrammarPrivate() {
        }

        void buildFirst(std::set<std::string> &first) {
            for (auto prod : productions) {

            }
        }

        std::vector<ProdPtr> productions;
    };

    Grammar::Grammar() :
    data(new GrammarPrivate()) {}

    Grammar::~Grammar() {
    }

    void Grammar::add(const std::string &head, const std::string &body) {
    }

    void Grammar::print(std::ostream &os)
    {
//        os << data->head << " -> " << bodyString();
    }

    std::string Grammar::toString() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    std::vector<std::string> FIRST() {

    }

    std::vector<std::string> FOLLOW() {

    }


    }
