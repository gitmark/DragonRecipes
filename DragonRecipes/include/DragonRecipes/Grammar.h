/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <DragonRecipes/Constants.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Production.h>

namespace dragon
{

class GrammarPrivate;

    class DRAGON_EXPORT Grammar
    {
    public:
        Grammar();
        virtual ~Grammar();

        void add(std::shared_ptr<Production> production);
        void add(std::shared_ptr<Symbol> symbol);

        int first(const std::string &x, std::set<std::string> &first);
        void print(std::ostream &os) const;
        std::string toString() const;

        const std::vector<std::string> &nontermVec() const;
        const std::vector<std::string> &termVec() const;
        const std::set<std::string> &nontermSet() const;
        const std::set<std::string> &termSet() const;

        int tokenId(const std::string &token) const;
        std::string tokenString(int id) const;

        int tokenType(int id) const;
        int tokenType(const std::string &token) const;

        void setTerminals(const std::vector<SymbolPtr> &terminals);
        void setNonterminals(const std::vector<SymbolPtr> &nonterminals);
        void setProductions(const std::map<std::string, std::vector<ProdPtr>> &productions);
        void setStartSymbol(const std::string& symbol);
        void updateMembers();

    protected:
        UniquePtr<GrammarPrivate> data;
    };

    typedef std::shared_ptr<Grammar> GrammarPtr;

    inline GrammarPtr newGrammar() {
        return std::make_shared<Grammar>();
    }
	

			
}

#endif

