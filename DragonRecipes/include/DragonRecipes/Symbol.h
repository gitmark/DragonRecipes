/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <memory>

#include <DragonRecipes/Constants.h>

namespace dragon
{


class SymbolPrivate;






    class DRAGON_EXPORT Symbol
    {
    public:
        enum Type {unknown, term, nonterm};

        Symbol(const std::string &name = "", int id = 0, Type type = unknown);
		Symbol::Symbol(UniquePtr<SymbolPrivate> &&ptr);
        virtual ~Symbol();
        
        int id() const;
        const std::string name() const;
        Type type() const;
        std::string typeStr() const;

        void setId(int id);
        void setName(const std::string &name);
        void setType(Type type);

        void print(std::ostream &os) const;
        std::string toString() const;
 

    protected:
        UniquePtr<SymbolPrivate> data;
    };


    typedef std::shared_ptr<Symbol> SymbolPtr;



    inline SymbolPtr newSymbol(const std::string &name = "", int id = 0, Symbol::Type type = Symbol::unknown) {
        return std::make_shared<Symbol>(name, id, type);
    }

    inline SymbolPtr newTerm(const std::string &name = "", int id = 0) {
        return std::make_shared<Symbol>(name, id, Symbol::term);
    }

    inline SymbolPtr newNonterm(const std::string &name = "", int id = 0) {
        return std::make_shared<Symbol>(name, id, Symbol::nonterm);
    }
}

#endif
