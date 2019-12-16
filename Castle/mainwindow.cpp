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
#include <DragonRecipes/Node.h>
#include "MainWindow.h"
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

int MainWindow::init(Config *config) {
    _config = config;

    for(auto &exp : _config->expressions)
        ui->listExpressions->addItem(exp.c_str());
    return 0;
}

int tree_depth(NodePtr &n, int &depth, int y = 1)
{
    if (y > depth)
        depth = y;

    for (auto &c : n->children())
    {
        tree_depth(c, depth, y + 1);
    }

    return depth;
}

#define COL_WIDTH 5
int print_at_depth(std::ostream &os, NodePtr &n, int depth, int radius, int y, int x, int &last_x, bool compact = true)
{
    if (y == depth)
    {
        os << std::string(static_cast<size_t>(x - last_x), ' ');

        int size = 0;
        if (n->children().empty()) {
            TokenPtr token = std::static_pointer_cast<Token>(n->symbol());
            if (token) {
                os << token->lexeme();
                size = static_cast<int>(token->lexeme().size());
            } else {
                os << "?";
                size = 1;
            }
        } else {
            os << n->symbol()->name();
            size = static_cast<int>(n->symbol()->name().size());
        }

        last_x = x + size;
        return 0;
    }

    int x_delta = -radius;
    for (auto &c : n->children())
    {
        if (compact)
            print_at_depth(os, c, depth, radius, y + 1, x + x_delta, last_x, compact);
        else
            print_at_depth(os, c, depth, radius/2, y + 1, x + x_delta, last_x, compact);
        x_delta += 2*radius;
    }

    return 0;
}

int print_edges_at_depth(std::ostream &os, NodePtr &n, int depth, int edge_radius, int radius, int y, int x, int &last_x, bool compact = true)
{
    if (y == depth)
    {
        x -= edge_radius;

        size_t pad = 0;
        if (x > last_x)
            pad = static_cast<size_t>(x - last_x);
        else
            os.flush(); // << "error\n";

        if (n->children().size() == 2)
        {
            os << std::string(pad, ' ');
            os << "/" << std::string(static_cast<size_t>((edge_radius-1)*2 + 1), ' ') << "\\";
        }
        else if (n->children().size() == 1)
        {
            os << std::string(pad, ' ');
            os << " | ";
        }
        else
        {
            os << std::string(pad, ' ');
            os << "   ";
        }

        last_x = x + 3;
        return 0;
    }

    int x_delta = -radius;
    for (auto &c : n->children())
    {
        if (compact)
            print_edges_at_depth(os, c, depth, edge_radius, radius, y + 1, x + x_delta, last_x, compact);
        else
            print_edges_at_depth(os, c, depth, edge_radius, radius/2, y + 1, x + x_delta, last_x, compact);
        x_delta += 2*radius;
    }

    return 0;
}

int print_tree(std::ostream &os, NodePtr &n, bool compact = true)
{
    int depth = 0;
    tree_depth(n, depth);

    int x_offset = 50;
    int radius = 1 << (depth - 1);
    if(compact)
        radius = 2;
    for (int i = 0; i < depth; ++i)
    {
        int last_x = 0;
        print_at_depth(os, n, i, radius, 0, x_offset, last_x, compact);
        os << "\n";
        int count = radius/(1<<(i));
        if(compact)
            count = 2;
        for(int edge_radius = 1; edge_radius < count; ++edge_radius)
        {
            int last_edge_x = 0;
            if(compact)
                print_edges_at_depth(os, n, i, edge_radius, radius, 0, x_offset, last_edge_x, compact);
            else
                print_edges_at_depth(os, n, i, edge_radius, radius, 0, x_offset, last_edge_x, compact);
            os << "\n";
        }
    }

    return 0;
}

class Expression;
class PrintStack {
public:
    PrintStack(Expression *exp, const std::string &scope);
    virtual ~PrintStack();

private:
    Expression *exp;
};

using PS = PrintStack;

class Expression {
public:
    Expression(std::ostream &os, LexerPtr lexer):
        os(os), lexer(lexer) {}

    NodePtr F() {
        PS ps(this, "F");
        if (lexer->token()->name() == "id") {
            TokenPtr tok = lexer->token();
            lexer->next();
            return newNode(tok);
        } else if (lexer->token()->name() == "(") {
            TokenPtr tok = lexer->token();
            lexer->next();
            NodePtr exp = E();
            lexer->next();
            return exp;
        }

        return nullptr;
    }

    // Maybe a * expression
    NodePtr T1(NodePtr input) {
        PS ps(this, "T1");

        if (lexer->token()->name() == "*") {
            NodePtr mult = newNode(newToken(lexer->token()->name(), lexer->token()->id(), lexer->token()->lexeme()));
            lexer->next();
            mult->addChild(input);
            mult->addChild(F());
            return T1(mult);
        } else {
            return input;
        }
    }

    // Returns a value
    NodePtr T() {
        PS ps(this, "T");
        return T1(F()); // Maybe a mult expression
    }

    // Maybe a + expression
    NodePtr E1(NodePtr input) {
        PS ps(this, "E1");

        if (lexer->token()->name() == "+") {
            NodePtr plus = newNode(newToken(lexer->token()->name(), lexer->token()->id(), lexer->token()->lexeme()));
            lexer->next();
            plus->addChild(input);
            plus->addChild(T());
            return E1(plus);
        } else {
            return input;
        }
    }

    NodePtr E() {
        PS ps(this, "E");

        if (lexer->token()->name() == "id") {
            return E1(T());
        }

        return nullptr;
    }

    void printStack(const std::string &postfix) {
        bool first = true;

        std::stringstream ss;
        for (auto s : stack) {
            if (first)
                first = false;
            else
                ss << " ";

            ss << s;
        }

        os << std::setw(20) << std::right << ss.str() << postfix;
        os << "\n";
    }

    void pushScope(const std::string &scope) {
        stack.push_front(scope);
    }

    void popScope() {
        stack.pop_front();
    }

private:
    std::ostream &os;
    LexerPtr lexer;
    std::deque<std::string> stack;
};

PrintStack::PrintStack(Expression *exp, const std::string &scope) :
    exp(exp) {
    exp->pushScope(scope);
    exp->printStack(" {");
}

PrintStack::~PrintStack() {
    exp->printStack(" }");
    exp->popScope();
}

using namespace log;
void MainWindow::on_pushButton_clicked() {
    ui->textEditResults->setFont (QFont ("Courier", 13));

    ui->textEditLog->setFont (QFont ("Courier", 13));

    log::error.setListener(newLogListener([&](const std::string &str) {
        ui->textEditLog->setText(ui->textEditLog->toPlainText() + str.c_str());
    }));

    /*
    TextCanvas canvas(10,10);
    canvas.drawLine(Point(1,1),Point(8,8));
    canvas.drawText(Point(1,1), "one");
    canvas.drawText(Point(8,8), "two");

    error << canvas.str();
*/

    LexerPtr lexer = newLexer();

    int id = 0;
    lexer->addTerminal("id", ++id, "[0-9]+");
    lexer->addTerminal("S", ++id, "[ \t]+");
    lexer->addTerminal("", ++id, "[-*/+()]");
    lexer->addTerminal("NAME", ++id, "[_a-zA-Z][_a-zA-Z0-9]*");

    std::string exp = ui->listExpressions->currentItem()->text().toStdString();
    lexer->setSource(exp);
//    lexer->setSource("1 + 2 * 3 + 4 * 5 * 6 * 7 + 2 * 3 * 4");

    //    lexer->setSource(ui->lineEditHead->text().toStdString());

//    lexer->next();
//    Expression exp(error, lexer);
//    NodePtr result = exp.E();
//    ui->treeGrid->addTree(result);
//    print_tree(error, result, false);

    std::string text;
    if (!_filename.empty()) {
        std::ifstream t(_filename);
        std::stringstream buffer;
        buffer << t.rdbuf();
        text = buffer.str();
    }

    std::vector<SymbolPtr> terminals;
    std::vector<SymbolPtr> nonterminals;

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

    grammar->add(newProduction("E", "T E1", [](std::vector<NodePtr> &nodes,TokenPtr token){ return 0;}));

    grammar->add(newProduction("E1", "+ T E1",
[](std::vector<NodePtr> &nodes,TokenPtr token){
                                   nodes.push_back(newNode(token));
return 0;}));
    grammar->add(newProduction("E1", "e",
[](std::vector<NodePtr> &nodes,TokenPtr token){

                                   if (nodes.size() < 3) {
                                       return 0;
                                   }

                                   TokenPtr t = std::dynamic_pointer_cast<Token>(nodes[nodes.size() - 2]->symbol());

                                   if (t && t->lexeme() == "(" && token->lexeme() == ")") {
                                       NodePtr n = nodes.back();
                                       nodes.pop_back();
                                       nodes.pop_back();
                                       nodes.push_back(n);
                                       return 0;
                                   }
                                   if (!t || t->lexeme() != "*") {
                                       return 0;
                                   }

                                   NodePtr right = nodes.back();
                                   nodes.pop_back();

                                   NodePtr op = nodes.back();
                                   nodes.pop_back();

                                   NodePtr left = nodes.back();
                                   nodes.pop_back();

                                   op->addChild(left);
                                   op->addChild(right);
                                   /*
                                   if (token->lexeme() == ")") {
                                       TokenPtr t2 = std::dynamic_pointer_cast<Token>(nodes.back()->symbol());
                                       if (t2 && t2->lexeme() == "(")
                                          nodes.pop_back();
                                   }
*/
                                   nodes.push_back(op);

return 0;}));

    grammar->add(newProduction("T", "F T1"));
    grammar->add(newProduction("T1", "* F T1",
                               [](std::vector<NodePtr> &nodes,TokenPtr token){
                                   nodes.push_back(newNode(token));
                                   return 0;}));

    grammar->add(newProduction("T1", "e",
[](std::vector<NodePtr> &nodes,TokenPtr token){

                                   if (nodes.size() < 2) {
                                       return 0;
                                   }

                                   TokenPtr t = std::dynamic_pointer_cast<Token>(nodes[nodes.size() - 2]->symbol());

                                   if (t && t->lexeme() == "(" && token->lexeme() == ")") {
                                       NodePtr n = nodes.back();
                                       nodes.pop_back();
                                       nodes.pop_back();
                                       nodes.push_back(n);
                                       return 0;
                                   }

                                   if (nodes.size() < 3) {
                                       return 0;
                                   }

                                   if (!t || t->lexeme() != "+") {
                                       return 0;
                                   }

                                   NodePtr right = nodes.back();
                                   nodes.pop_back();

                                   NodePtr op = nodes.back();
                                   nodes.pop_back();

                                   NodePtr left = nodes.back();
                                   nodes.pop_back();

                                   op->addChild(left);
                                   op->addChild(right);
                                   if (token->lexeme() == ")") {
                                       TokenPtr t2 = std::dynamic_pointer_cast<Token>(nodes.back()->symbol());
                                       if (t2 && t2->lexeme() == "(")
                                          nodes.pop_back();
                                   }
                                   nodes.push_back(op);



return 0;}));
    grammar->add(newProduction("F", "( E )",
[](std::vector<NodePtr> &nodes,TokenPtr token){
                                   nodes.push_back(newNode(token));
return 0;}));

    grammar->add(newProduction("F", "id",
                                            [](std::vector<NodePtr> &nodes,TokenPtr token){

                                   NodePtr right = newNode(token);
                                   if (nodes.size() < 2) {
                                       nodes.push_back(right);
                                       return 0;
                                   }

                                   TokenPtr t = std::dynamic_pointer_cast<Token>(nodes.back()->symbol());
                                   if (!t || t->lexeme() != "*") {
                                       nodes.push_back(right);
                                       return 0;
                                   }

                                   NodePtr op = nodes.back();
                                   nodes.pop_back();

                                   NodePtr left = nodes.back();
                                   nodes.pop_back();

                                   op->addChild(left);
                                   op->addChild(right);
                                   nodes.push_back(op);

return 0;}));
    grammar->setStartSymbol("E");
    rc = grammar->buildTables();

    if (rc != E_SUCCESS) {
        log::error << errorString(rc) << "\n";
        return;
    }

    std::string head; // = ui->lineEditHead->text().toStdString();
    std::string body; // = ui->lineEditBody->text().toStdString();
    production = std::make_shared<Production>(head, body);
    std::vector<std::string> parts = production->bodyVec();
    std::stringstream ss;

    ss << production->head() << "\n\n";

    for(auto const &p : parts) {
        ss << p << "\n";
    }

    ui->textEditResults->setText(grammar->toString().c_str());
    log::error.flush();

    NodePtr n = grammar->runPredictiveParser(error,lexer);
    ui->treeGrid->addTree(n);
    error << ui->treeGrid->textTree() << "\n";

}

}

void castle::MainWindow::on_listExpressions_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    on_pushButton_clicked();
}
