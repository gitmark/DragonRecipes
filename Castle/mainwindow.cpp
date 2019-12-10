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

void MainWindow::on_pushButton_clicked() {

//    log::info.setListener(newLogListener([&](const std::string &str){
//        ui->textEditLog->setText(ui->textEditLog->toPlainText() + str.c_str());
//    }));

    log::error.setListener(newLogListener([&](const std::string &str) {
        ui->textEditLog->setText(ui->textEditLog->toPlainText() + str.c_str());
    }));

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
    grammar->add(newProduction("E", "T E1"));
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
}

}
