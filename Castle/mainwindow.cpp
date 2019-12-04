/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Grammar.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Log.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace dragon;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    log::info.setListener(newLogListener([&](const std::string &str){
        ui->textEditLog->setText(ui->textEditLog->toPlainText() + str.c_str());
    }));

    log::info << "hey";
    log::info.flush();

    std::vector<SymbolPtr> terminals;

    terminals.push_back(newTerm("a", 1));
    terminals.push_back(newTerm("b", 2));
    terminals.push_back(newTerm("c", 3));

    std::vector<SymbolPtr> nonterminals;

    nonterminals.push_back(newNonterm("A", 101));
    nonterminals.push_back(newNonterm("B", 102));
    nonterminals.push_back(newNonterm("C", 103));

    GrammarPtr grammar = newGrammar();

    grammar->setTerminals(terminals);
    grammar->setNonterminals(nonterminals);
    grammar->add(newProduction("A", "a A"));
    grammar->add(newProduction("A", "b A"));
    grammar->add(newProduction("B", "c"));
    grammar->add(newProduction("C", "B A"));
    grammar->setStartSymbol("A");
    grammar->updateMembers();

    std::string head = ui->lineEditHead->text().toStdString();
    std::string body = ui->lineEditBody->text().toStdString();
    production = std::make_shared<Production>(head, body);
    std::vector<std::string> parts = production->bodyVec();
    std::stringstream ss;
    ss << production->head() << "\n\n";
    for(auto p : parts) {
        ss << p << "\n";
    }

    ui->textEditResults->setText(grammar->toString().c_str());
}

