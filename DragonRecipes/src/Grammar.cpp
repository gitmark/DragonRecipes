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

    class GrammarPrivate
    {
    public:
        GrammarPrivate() {
        }

         int firstInBody(const std::string &body, std::set<std::string> &firstSet) {
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

        int first(const std::string &x, std::set<std::string> &firstSet) {

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
    }

    std::string Grammar::toString() const {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    int Grammar::first(const std::string &x, std::set<std::string> &first) {

        return 0;
    }

    const std::vector<std::string> &Grammar::nontermVec() const {
        return data->nontermVec;
    }

    const std::vector<std::string> &Grammar::termVec() const {
        return data->termVec;
    }

    void Grammar::setTerminals(const std::vector<SymbolPtr> &terminals) {
        data->terminals = terminals;
    }

    void Grammar::setNonterminals(const std::vector<SymbolPtr> &nonterminals) {
        data->nonterminals = nonterminals;
    }

    void Grammar::setProductions(const std::map<std::string, std::vector<ProdPtr>> &productions) {
        data->productions = productions;
    }

    void Grammar::setStartSymbol(const std::string& symbol) {
        data->startSymbol = symbol;
    }

    void Grammar::updateMembers() {

    }


    }
