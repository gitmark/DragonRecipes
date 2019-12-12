/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Grammar.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Log.h>
#include <DragonRecipes/Error.h>
#include <DragonRecipes/Lexer.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace dragon;

namespace castle {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::init(const std::string &filename) {
    _filename = filename;
    return 0;
}

int F(LexerPtr lexer);
int A2(LexerPtr lexer, int a, int b);
int M2(LexerPtr lexer, int a, int b);
int X(LexerPtr lexer);

int A2(LexerPtr lexer, int a, int b) {
    if (lexer->token()->name() == ")") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        return a+b;
    } else if (lexer->token()->name() == "+") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return A2(lexer, a+b, val);
    } else if (lexer->token()->name() == "*") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return a + M2(lexer, b, val);
    }
    else {
        return a+b;
    }
}

int M2(LexerPtr lexer, int a, int b) {
    if (lexer->token()->name() == ")") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        return a*b;
    } else if (lexer->token()->name() == "+") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return A2(lexer, a*b, val);
    } else if (lexer->token()->name() == "*") {
            do {
                lexer->next();
            } while(lexer->token()->id() == 2);

            int val = F(lexer);
            return M2(lexer, a*b, val);
        }

    else {
        return a*b;
    }
}

int N(LexerPtr lexer, int input) {
    if (lexer->token()->name() == ")") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        return input;
    } if (lexer->token()->name() == "*") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return M2(lexer, input, val);
    } else if (lexer->token()->name() == "+") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return A2(lexer, input, val);
    } else {
        return input;
    }
}

int X(LexerPtr lexer) {
    int val = F(lexer);
    return N(lexer, val);
}


int F(LexerPtr lexer) {
    if (lexer->token()->name() == "id") {
        TokenPtr tok = lexer->token();
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = std::atoi(tok->lexeme().c_str());
        return val;
    } else if (lexer->token()->name() == "(") {
        TokenPtr tok = lexer->token();
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        return X(lexer);
    }

    return 0;
}

int T1(LexerPtr lexer, int input) {
    if (lexer->token()->name() == "*") {
        do {
            lexer->next();
        } while(lexer->token()->id() == 2);

        int val = F(lexer);
        return input * T1(lexer, val);
    } else {
        return input;
    }
}

int T(LexerPtr lexer) {
    if (lexer->token()->name() == "id") {
        int val = F(lexer);
        return T1(lexer, val);
    }

    return 0;
}

int E1(LexerPtr lexer, int input) {
    if (lexer->token()->name() == "+") {
        do {
        lexer->next();
        } while(lexer->token()->id() == 2);
        int val = T(lexer);
        return input + E1(lexer, val);
    } else {
        return input;
    }
}



int E(LexerPtr lexer) {
    if (lexer->token()->name() == "id") {
        int val = T(lexer);
        return E1(lexer, val);
    }

    return 0;
}

using namespace log;
void MainWindow::on_pushButton_clicked() {

    log::error.setListener(newLogListener([&](const std::string &str) {
        ui->textEditLog->setText(ui->textEditLog->toPlainText() + str.c_str());
    }));


    LexerPtr lexer = newLexer();

    int id = 0;
    lexer->addTerminal("id", ++id, "[0-9]+");
    lexer->addTerminal("S", ++id, "[ \t]+");
    lexer->addTerminal("", ++id, "[-*/+()]+");
    lexer->addTerminal("NAME", ++id, "[_a-zA-Z][_a-zA-Z0-9]*");

    lexer->setSource("(1+2) * 3");

    lexer->next();
    int result = X(lexer);

    /*
    while (lexer->next()) {
        TokenPtr token = lexer->token();
        error << token->name() << ", " << token->id() << "\n";
    }
*/

    std::string text;
    if (!_filename.empty()) {
        std::ifstream t(_filename);
        std::stringstream buffer;
        buffer << t.rdbuf();
        text = buffer.str();
    }

//    log::info << text;
//    log::info.flush();

    std::vector<SymbolPtr> terminals;
    std::vector<SymbolPtr> nonterminals;

    /*
    terminals.push_back(newTerm("a", 1));
    terminals.push_back(newTerm("b", 2));
    terminals.push_back(newTerm("c", 3));

    nonterminals.push_back(newNonterm("A", 101));
    nonterminals.push_back(newNonterm("B", 102));
    nonterminals.push_back(newNonterm("C", 103));
    */

//    std::vector<std::string> terminals = {"id", "+", "*", "(", ")", "$"};
//    std::vector<std::string> nonterminals = {"E", "E1", "T", "T1", "F"};

    terminals.push_back(newTerm("id", 1));
    terminals.push_back(newTerm("+", 2));
    terminals.push_back(newTerm("*", 3));
    terminals.push_back(newTerm("(", 4));
    terminals.push_back(newTerm(")", 5));
    terminals.push_back(newTerm("$", 6));

    nonterminals.push_back(newNonterm("E", 101));
    nonterminals.push_back(newNonterm("E1", 102));
    nonterminals.push_back(newNonterm("T", 103));
    nonterminals.push_back(newNonterm("T1", 104));
    nonterminals.push_back(newNonterm("F", 105));

    GrammarPtr grammar = newGrammar();

    grammar->setTerminalRange(1,6);
    grammar->setNonterminalRange(101,105);

    int rc = grammar->setTerminals(terminals);

    if (rc != E_SUCCESS) {
        log::error << errorString(rc) << "\n";
        return;
    }

    rc = grammar->setNonterminals(nonterminals);

    if (rc != E_SUCCESS) {
        log::error << errorString(rc) << "\n";
        return;
    }

    /*
    grammar->add(newProduction("A", "a A"));
    grammar->add(newProduction("A", "b A"));
    grammar->add(newProduction("B", "c"));
    grammar->add(newProduction("C", "B A"));
*/
    grammar->add(newProduction("E", "T E1", [&](){return 1;}));
    grammar->add(newProduction("E1", "+ T E1"));
    grammar->add(newProduction("E1", "e"));
    grammar->add(newProduction("T", "F T1"));
    grammar->add(newProduction("T1", "* F T1"));
    grammar->add(newProduction("T1", "e"));
    grammar->add(newProduction("F", "( E )"));
    grammar->add(newProduction("F", "id"));
    grammar->setStartSymbol("E");
    rc = grammar->buildTables();

    if (rc != E_SUCCESS) {
        log::error << errorString(rc) << "\n";
        return;
    }

    std::string head = ui->lineEditHead->text().toStdString();
    std::string body = ui->lineEditBody->text().toStdString();
    production = std::make_shared<Production>(head, body);
    std::vector<std::string> parts = production->bodyVec();
    std::stringstream ss;

    ss << production->head() << "\n\n";

    for(auto const &p : parts) {
        ss << p << "\n";
    }

    ui->textEditResults->setText(grammar->toString().c_str());
    log::error.flush();
    ui->textEditResults->setFont (QFont ("Courier", 13));

    ui->textEditLog->setFont (QFont ("Courier", 13));

    grammar->runPredictiveParser(error,lexer);
}

}
