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
#include <DragonRecipes/Log.h>

//using namespace std;

namespace dragon {
bool compareSymbol(const SymbolPtr &sp1, const SymbolPtr &sp2) {
    return (sp1->id() < sp2->id());
}

class GrammarPrivate {
  public:

    int firstInBody(const std::string &body, std::set<std::string> &firstSet) const {
        std::vector<std::string> parts;
        split(body, " ", parts);

        for (size_t i = 0; i < parts.size(); ++i) {
            auto p = parts[i];

            std::set<std::string> first1;
            first(p, first1);

            // Add everything except e (empty)
            for(const auto &f : first1) {
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
        if (x.find(' ') != std::string::npos)
            return firstInBody(x, firstSet);

        if (!m.count(x)) {
            // terminal or e, thus add to first
            firstSet.insert(x);
            return 0;
        }

        // nonterminal, get productions
        auto vec = m.at(x);

        for(const auto &prod : vec) {
            firstInBody(prod->body(), firstSet);
        }

        return 0;
    }

    int follow(const std::string &x, std::set<std::string> &followSet, const std::string &startSymbol1) const {

        const std::map<std::string, std::vector<ProdPtr>> &m = productions;
        if (x == startSymbol1)
            followSet.insert("$");

        for (auto mpair : m) {
            for(auto prod : mpair.second) {
                std::vector<std::string> parts;
                split(prod->body(), " ", parts);

                for (size_t i = 0; i < parts.size(); ++i) {
                    auto p = parts[i];

                    if (p == x) {

                        for (size_t j = i; j < parts.size(); ++j) {
                            auto p = parts[j];

                            if (p != x) {

                                std::set<std::string> first1;
                                first(p, first1);

                                // Add everything except e
                                for(auto f : first1) {
                                    if (f != "e")
                                        followSet.insert(f);
                                }

                                if (!first1.count("e")) {
                                    break;
                                }
                            }

                            if (j == parts.size() - 1) {
                                if (prod->head() != p) {
                                    follow(prod->head(), followSet, startSymbol1);
                                }
                                break;
                            }
                        }

                        break;
                    }
                }
            }
        }

        return 0;
    }

    int createTable() {
        table.clear();
        const std::map<std::string, std::vector<ProdPtr>> &m = productions;

        for (auto nonterm : nontermVec) {
            auto vec = m.at(nonterm);

            std::set<std::string> first1;
            first(nonterm, first1);
            std::set<std::string> follow1;
            follow(nonterm, follow1, startSymbol);

            ProdPtr prod1;
            ProdPtr prod2;

            std::map<std::string, ProdPtr> termToProd;

            for(auto prod : vec) {

                std::set<std::string> bodyFirst;
                first(prod->body(), bodyFirst);

                for (auto t : termVec) {
                    if (bodyFirst.count(t)) {
                        termToProd[t] = prod;
                    }
                }

                if (prod->body() == "e")
                    prod2 = prod;
                else
                    prod1 = prod;
            }

            for (auto term : termVec) {

                if (prod2) {
                    if (follow1.count(term)) {
                        table[nonterm][term] = prod2;
                    }
                }

                if (first1.count(term)) {
                    table[nonterm][term] = termToProd[term];
                }
            }
        }
    }

    int printTable(std::ostream &os) {

        std::vector<std::string> terminals = {"id", "+", "*", "(", ")", "$"};
        std::vector<std::string> nonterminals = {"E", "E1", "T", "T1", "F"};

        int width = 12;
        std::string padding1(width, ' ');
        std::string line((width+1)*(terminals.size() + 1), '-');
        os << line << "\n";
        os << padding1 << "|";
        for (auto term : terminals) {
            std::string leftPadding((width - term.size())/2, ' ');
            std::string entry = leftPadding + term;
            os << entry;
            std::string padding(width - entry.size(), ' ');
            os << padding << "|";
        }
        os << "\n";

        for (auto nonterm : nonterminals) {
            os << line << "\n";
            std::string leftPadding((width - nonterm.size())/2, ' ');
            std::string entry = leftPadding + nonterm;
            os << entry;
            std::string padding(width - entry.size(), ' ');
            os << padding << "|";
            for (auto term : terminals) {
                ProdPtr prod;
                if (table.at(nonterm).count(term))
                    prod = table.at(nonterm).at(term);
                std::string val;
                std::string entry;
                if (prod) {
                    val = nonterm + "->";
                    std::vector<std::string> vec;
                    split(prod->body(), " ", vec);
                    std::string body;
                    for (auto item : vec) {
                        body += item;
                    }
                    val += body;
                    std::string leftPadding((width - val.size())/2, ' ');
                    entry = leftPadding + val;
                }

                os << entry;
                std::string padding(width - entry.size(), ' ');
                os << padding << "|";
            }
            os << "\n";
        }
        os << line << "\n";

        os << "\n";

        return 0;
    }


    void add(const std::string &head, const std::string &body) {
        productions[head].push_back(newProduction(head,body));
        nontermSet.insert(head);
    }

private:
    std::map<std::string, std::map<std::string, ProdPtr>> table;
    std::map<std::string, std::vector<ProdPtr>> productions;
    std::vector<std::string> nontermVec;
    std::vector<std::string> termVec;
    std::set<std::string> nontermSet;
    std::set<std::string> termSet;
    std::vector<SymbolPtr> terminals;
    std::vector<SymbolPtr> nonterminals;
    std::string startSymbol;
    int termFirstId{-1};
    int termLastId{-1};
    int nontermFirstId{-1};
    int nontermLastId{-1};
    ErrorPtr lastError;

    friend class Grammar;
};


Grammar::Grammar() :
    data(new GrammarPrivate()) {}


Grammar::Grammar(std::unique_ptr<GrammarPrivate> &&ptr) :
    data(std::move(ptr)) {}

Grammar::Grammar(const Grammar &grammar) :
    data(new GrammarPrivate()) {
    *data = *grammar.data;
}

Grammar::Grammar(Grammar &&grammar) noexcept :
    data(new GrammarPrivate()) {
    std::swap(data, grammar.data);
}

Grammar &Grammar::operator=(const Grammar &grammar) {
    *data = *grammar.data;
    return *this;
}

Grammar &Grammar::operator=(Grammar &&grammar) noexcept {
    std::swap(data, grammar.data);
    return *this;
}

Grammar::~Grammar() {
    data.reset(nullptr);
}

int Grammar::createTable() {
    return data->createTable();
}

int Grammar::printTable(std::ostream &os) {
    return data->printTable(os);
}

void Grammar::add(const std::shared_ptr<Production> &production) {
    data->productions[production->head()].push_back(production);
}

void Grammar::add(const SymbolPtr &symbol) {
    if (symbol->type() == Symbol::term)
        data->terminals.push_back(symbol);
    else if (symbol->type() == Symbol::term)
        data->terminals.push_back(symbol);
    else {
        std::cerr << "error Grammar::add() - unknown symbol type\n";
    }
}

void Grammar::print(std::ostream &os) {
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
            for (const auto &symbol : body) {
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

    os << "\n";

    // Print FOLLOW sets

    for(auto &nonterm : nontermVec()) {
        os << "FOLLOW(" << nonterm << ") =";
        std::set<std::string> follow1;
        follow(nonterm, follow1, startSymbol());

        for(auto &symbol : follow1) {
            os << " ";
            os << symbol;
        }

        os << "\n";
    }

    createTable();
    printTable(os);


}

std::string Grammar::toString() {
    std::stringstream ss;
    print(ss);
    return ss.str();
}

int Grammar::first(const std::string &x, std::set<std::string> &first) const {
    return data->first(x, first);
}


int Grammar::follow(const std::string &x, std::set<std::string> &followSet, const std::string &startSymbol1) const {
    return data->follow(x,followSet,startSymbol1);
}

const std::vector<std::string> &Grammar::nontermVec() const {
    return data->nontermVec;
}

const std::vector<std::string> &Grammar::termVec() const {
    return data->termVec;
}

std::string Grammar::startSymbol() const {
    return data->startSymbol;
}

int Grammar::setTerminals(const std::vector<SymbolPtr> &terminals) {
    data->terminals = terminals;
    data->termSet.clear();
    data->termVec.clear();

    if(terminals.empty())
        return E_SUCCESS;

    std::sort(data->terminals.begin(), data->terminals.end(), compareSymbol);

    if (data->termFirstId >= 0 && data->terminals.front()->id() < data->termFirstId) {
        log::error << "terminal out of range: " << data->terminals.front()->id() << " is less than the min id " << data->termFirstId << "\n";
        return E_TERM_OUT_OF_RANGE;
    }

    if (data->termLastId >= 0 && data->terminals.back()->id() > data->termLastId) {
        log::error << "terminal out of range: " << data->terminals.back()->id() << " is greater than the max id " << data->termLastId << "\n";
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

    if(nonterminals.empty())
        return E_SUCCESS;

    std::sort(data->nonterminals.begin(), data->nonterminals.end(), compareSymbol);

    if ((data->nontermFirstId >= 0) && (data->nonterminals.front()->id() < data->nontermFirstId)) {
        log::error << "nonterminal out of range: " << data->nonterminals.front()->id() << " is less than the min id " << data->nontermFirstId << "\n";
        return E_NONTERM_OUT_OF_RANGE;
    }

    if ((data->nontermLastId >= 0) && (data->nonterminals.back()->id() > data->nontermLastId)) {
        log::error << "nonterminal out of range: " << data->nonterminals.back()->id() << " is greater than the max id " << data->nontermLastId << "\n";
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

int Grammar::buildTables() {
    std::string scope =  "Grammar::buildTables() - ";
    if (startSymbol() == "") {
        log::error << scope << "start symbol is not set.\n";
        return E_START_SYMBOL_NOT_SET;
    }
    return E_SUCCESS;
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
