/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <map>
#include <DragonRecipes/Constants.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Production.h>
#include <DragonRecipes/Error.h>
#include <DragonRecipes/Lexer.h>

namespace dragon {

class GrammarPrivate;

class DRAGON_EXPORT Grammar {
  public:
    enum Error {None, TermOutOfRange, NontermOutOfRange, UnknownGrammar};
    Grammar();
    Grammar(std::unique_ptr<GrammarPrivate> &&ptr);
    Grammar(const Grammar &grammar);
    Grammar(Grammar &&grammar) noexcept;
    Grammar &operator=(const Grammar &grammar);
    Grammar &operator=(Grammar &&Grammar) noexcept;

    virtual ~Grammar();

    void add(const std::shared_ptr<Production> &production);
    void add(const std::shared_ptr<Symbol> &symbol);

    int first(const std::string &x, std::set<std::string> &first) const;
    int follow(const std::string &x, std::set<std::string> &followSet, const std::string &startSymbol) const;

    void print(std::ostream &os);
    std::string toString();

    const std::vector<std::string> &nontermVec() const;
    const std::vector<std::string> &termVec() const;
    const std::set<std::string> &nontermSet() const;
    const std::set<std::string> &termSet() const;

    int tokenId(const std::string &token) const;
    std::string tokenString(int id) const;

    int tokenType(int id) const;
    int tokenType(const std::string &token) const;

    std::string startSymbol() const;
    int setTerminals(const std::vector<SymbolPtr> &terminals);
    int setNonterminals(const std::vector<SymbolPtr> &nonterminals);
    void setProductions(const std::map<std::string, std::vector<ProdPtr>> &productions);
    void setStartSymbol(const std::string& symbol);
    int buildTables();
    void setTerminalRange(int firstId, int lastId);
    void setNonterminalRange(int firstId, int lastId);
    int createTable();
    int printTable(std::ostream &os);

    ErrorPtr lastError();
    void clearLastError();

    NodePtr runPredictiveParser(std::ostream &os, LexerPtr lex);

  private:
      std::unique_ptr<GrammarPrivate> data;
};

using GrammarPtr = std::shared_ptr<Grammar>;

inline GrammarPtr newGrammar() {
    return std::make_shared<Grammar>();
}

}

#endif

