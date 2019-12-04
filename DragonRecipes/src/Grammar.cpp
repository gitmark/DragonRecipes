/*--------------------------------------------------------------------------
 Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
 distributed under the MIT License. See LICENSE.TXT for details.
--------------------------------------------------------------------------*/

#include <iostream>
#include <set>
#include <map>
#include <DragonRecipes/Grammar.h>
#include <DragonRecipes/StringTools.h>
#include <DragonRecipes/Production.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>

namespace dragon
    {
    bool compareSymbol(const SymbolPtr &sp1, const SymbolPtr &sp2)
    {
        return (sp1->id() < sp2->id());
    }

    class GrammarPrivate
    {
    public:
        GrammarPrivate() :
            termFirstId(-1),
            termLastId(-1),
            nontermFirstId(-1),
            nontermLastId(-1)
        {}

         int firstInBody(const std::string &body, std::set<std::string> &firstSet) const {
            std::vector<std::string> parts;
            split(body, " ", parts);

            for (size_t i = 0; i < parts.size(); ++i) {
                auto p = parts[i];

                std::set<std::string> first1;
                first(p, first1);

                // Add everything except e (empty)
                for(auto f : first1) {
                    if (f != "e")
                        firstSet.insert(f);
                }

                if (first1.count("e")) {
                    // If first1 contains e, add only if this is the last item
                    if (i == parts.size() - 1)
                        firstSet.insert("e"); // this is the last entry, thus there is nothing to continue to
                    // else continue to the next entry
                } else {
                    // If first1 doesnt contain e, break from loop
                    break;
                }
            }

            return 0;
        }

        int first(const std::string &x, std::set<std::string> &firstSet) const {

            auto &m = productions;
            if (x.find(" ") != std::string::npos)
                return firstInBody(x, firstSet);

            if (!m.count(x)) {
                // terminal or e, thus add to first
                firstSet.insert(x);
                return 0;
            }

            // nonterminal, get productions
            auto vec = m.at(x);

            for(auto prod : vec) {
                firstInBody(prod->body(), firstSet);
            }

            return 0;
        }

        void add(const std::string &head, const std::string &body) {
            productions[head].push_back(newProduction(head,body));
            nontermSet.insert(head);
        }

        std::map<std::string, std::vector<ProdPtr>> productions;
        std::vector<std::string> nontermVec;
        std::vector<std::string> termVec;
        std::set<std::string> nontermSet;
        std::set<std::string> termSet;
        std::vector<SymbolPtr> terminals;
        std::vector<SymbolPtr> nonterminals;
        std::string startSymbol;
        int termFirstId;
        int termLastId;
        int nontermFirstId;
        int nontermLastId;
        ErrorPtr lastError;
    };

	UNIQUE_PTR_IMPL(GrammarPrivate)
	
    Grammar::Grammar() :
    data(new GrammarPrivate()) {}

    Grammar::~Grammar() {
    }

    void Grammar::add(std::shared_ptr<Production> production) {
        data->productions[production->head()].push_back(production);
    }

    void Grammar::add(SymbolPtr symbol) {
        if (symbol->type() == Symbol::term)
            data->terminals.push_back(symbol);
        else if (symbol->type() == Symbol::term)
              data->terminals.push_back(symbol);
        else {
            std::cerr << "error Grammar::add() - unknown symbol type\n";
        }
    }

    void Grammar::print(std::ostream &os) const
    {
        // Print terminals
        os << "terminals:\n";
        for(auto &term : data->terminals) {
            os << term->name() << ": " << term->id() << "\n";
        }

        os << "\n";

        // Print nonterminals
        os << "nonterminals:\n";
        for(auto &term : data->nonterminals) {
            os << term->name() << ": " << term->id() << "\n";
        }

        os << "\n";

        // Print productions
        os << "productions:\n";
        for(auto &pair : data->productions) {
            auto &vec = pair.second;
            for(auto &prod : vec) {
            os << prod->head() << " -> " ;
            auto body = prod->bodyVec();
            bool first = true;
            for (auto symbol : body) {
                if (first) {
                    first = false;
                } else {
                  os << " ";
                }
                os << symbol;
            }
            os << "\n";
            }
        }

        os << "\n";

        // Print FIRST sets

        for(auto &nonterm : nontermVec()) {
        os << "FIRST(" << nonterm << ") =";
        std::set<std::string> first1;
        first(nonterm, first1);

        for(auto &symbol : first1) {
            os << " ";
            os << symbol;
        }

        os << "\n";
        }
    }

    std::string Grammar::toString() const {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    int Grammar::first(const std::string &x, std::set<std::string> &first) const {
        return data->first(x, first);
    }

    const std::vector<std::string> &Grammar::nontermVec() const {
        return data->nontermVec;
    }

    const std::vector<std::string> &Grammar::termVec() const {
        return data->termVec;
    }

    int Grammar::setTerminals(const std::vector<SymbolPtr> &terminals) {
        data->terminals = terminals;
        data->termSet.clear();
        data->termVec.clear();

        if(!terminals.size())
            return E_SUCCESS;

        std::sort(data->terminals.begin(), data->terminals.end(), compareSymbol);

        if (data->termFirstId >= 0 && data->terminals.front()->id() < data->termFirstId) {
            return E_TERM_OUT_OF_RANGE;
        }

        if (data->termFirstId >= 0 && data->terminals.back()->id() > data->termLastId) {
            return E_TERM_OUT_OF_RANGE;
        }

        for (auto &symbol : data->terminals) {
            data->termVec.push_back(symbol->name());
            data->termSet.insert(symbol->name());
        }

        return E_SUCCESS;
    }

    int Grammar::setNonterminals(const std::vector<SymbolPtr> &nonterminals) {
        data->nonterminals = nonterminals;
        data->nontermSet.clear();
        data->nontermVec.clear();

        if(!nonterminals.size())
            return E_SUCCESS;

        std::sort(data->nonterminals.begin(), data->nonterminals.end(), compareSymbol);

        if (data->nontermFirstId >= 0 && data->nonterminals.front()->id() < data->nontermFirstId) {
            return E_NONTERM_OUT_OF_RANGE;
        }

        if (data->nontermFirstId >= 0 && data->nonterminals.back()->id() > data->nontermLastId) {
            return E_NONTERM_OUT_OF_RANGE;
        }

        for (auto &symbol : data->nonterminals) {
            data->nontermVec.push_back(symbol->name());
            data->nontermSet.insert(symbol->name());
        }

        return E_SUCCESS;
    }

    void Grammar::setProductions(const std::map<std::string, std::vector<ProdPtr>> &productions) {
        data->productions = productions;
    }

    void Grammar::setStartSymbol(const std::string& symbol) {
        data->startSymbol = symbol;
    }

    void Grammar::updateMembers() {

    }

    void Grammar::setTerminalRange(int firstId, int lastId) {
        data->termFirstId = firstId;
        data->termLastId = lastId;
    }

    void Grammar::setNonterminalRange(int firstId, int lastId) {
        data->nontermFirstId = firstId;
        data->nontermLastId = lastId;
    }

    ErrorPtr Grammar::lastError() {
        return data->lastError;
    }

    void Grammar::clearLastError() {
        data->lastError = nullptr;
    }

    }
