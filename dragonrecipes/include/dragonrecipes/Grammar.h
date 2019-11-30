/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <memory>

#include <dragonrecipes/Constants.h>

namespace dr
{
class GrammarPrivate;

    class DRAGON_EXPORT Grammar
    {
    public:
        Grammar();
        Grammar(const Grammar&) = delete;
        Grammar(Grammar&&) = delete;
        virtual ~Grammar();

        void add(const std::string &head, const std::string &body);
        std::vector<std::string> FIRST();
        std::vector<std::string> FOLLOW();

        void print(std::ostream &os);
        std::string toString();

    protected:
        std::unique_ptr<GrammarPrivate> data;
    };
}

#endif

