/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include "TreeGrid.h"
#include <QPaintEvent>
#include <QPainter>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/TextPainter.h>
#include "QtPainter.h"

using namespace dragon;

namespace castle {

TreeGrid::TreeGrid(QWidget *parent) : QWidget(parent)
{
    grid.resize(height);
    for (size_t i = 0; i < height; ++i)
        grid[i].resize(width);
}

std::string TreeGrid::textTree() {
    return _treeText;
}


void TreeGrid::addTree(NodePtr node) {
    trees.push_back(newNodeDim(node));
    repaint();
}

void TreeGrid::addToGrid(int row, int col, SymbolPtr symbol) {
    TokenPtr token = std::dynamic_pointer_cast<Token>(symbol);
    if (token) {
        grid[row][col].str = token->lexeme();
    } else {
        grid[row][col].str = symbol->name();
    }

    grid[row][col].usage++;
}

void TreeGrid::addToGrid(int row, int col, NodePtr node) {
    NodeDimPtr nodeDim = newNodeDim(node);
    addToGrid(row, col, nodeDim);
}

void TreeGrid::addToGrid(int row, int col, NodeDimPtr nodeDim) {

    addToGrid(row, col, nodeDim->node()->symbol());

    SymbolPtr vertBar = newToken("", 1, "|");
    SymbolPtr horizBar = newToken("", 1, "-");
    for (auto ch : nodeDim->children()) {
        Vec edge = ch->parentEdge();

        for (int i = 1; i < edge.y; ++i) {
            addToGrid(row + i, col, vertBar);
        }

        for (int i = 1; i < edge.x; ++i) {
            addToGrid(row, col + i, horizBar);
        }

        addToGrid(row + edge.y, col + edge.x, ch);
    }
}

void TreeGrid::paintEvent(QPaintEvent *)
{
/*
    NodePtr n1 = newNode(newToken("",1,"*"));
    n1->addChild(newNode(newToken("N",2, "1")));
    n1->addChild(newNode(newToken("N",2, "2")));

    NodePtr n2 = newNode(newToken("",1,"*"));
    n2->addChild(newNode(newToken("N",2, "3")));
    n2->addChild(newNode(newToken("N",2, "4")));

    NodePtr n3 = newNode(newToken("",1,"+"));
    n3->addChild(n1);
    n3->addChild(n2);
*/
    if(trees.empty())
        return;

    NodeDimPtr nodeDim = trees.back();

    QPainter painter(this);

    painter.setRenderHint( QPainter::Antialiasing );
    painter.setBrush(QBrush(QColor(0,200,200)));

    QSize s = this->geometry().size();
    int winWidth = s.width();
    int winHeight = s.height();
    QRect r(0,0,winWidth, winHeight);
    painter.fillRect(r, QBrush(QColor(0,0,0)));
    int w = size().width() - 3;
    int h = size().height() - 5;

    painter.setPen(QPen(QColor(200,200,200)));
    painter.setBrush(QBrush(QColor(0,200,200)));
    int pointSize = 12;
    painter.setFont(QFont("Courier", pointSize));
    float scale = 10.0f;
    float fontSize = pointSize/1.7f;

    Rect canvasRect1(1e20f, 1e20f, -1e20f, -1e20f);

    nodeDim->paint(newPainter(painter), canvasRect1, Point(20, 1), Point(20, 1), scale, fontSize);
    Rect canvasRect(1e20f, 1e20f, -1e20f, -1e20f);

//    nodeDim->paint(painter, canvasRect, Point(20 - canvasRect1.x1, 1), Point(20 - canvasRect1.x1, 1), scale, fontSize);
    TextCanvas textCanvas;
    nodeDim->paint(newPainter(textCanvas), canvasRect, Point(20 - canvasRect1.x1, 1), Point(20 - canvasRect1.x1, 1), 1, 0);
    _treeText = textCanvas.str();
    /*
    // Draw cells
    for(size_t row = 0; row <= height; ++row) {
        for(size_t col = 0; col <= width; ++col) {
            if (row < height && col < width) {
            Cell &cell = grid[row][col];
            if (cell.usage > 0) {
                QRect rect(col*scale, row*scale, scale, scale);
                painter.fillRect(rect,QColor(100,100,100));
                painter.drawText(col*scale + (scale - fontSize)/2, row*scale + (fontSize + scale)/2, cell.str.c_str());
            }
            }
        }
    }

    // Draw grid
    for(size_t row = 0; row <= height; ++row) {
        painter.drawLine(0, row*scale, width*scale, row*scale);
        for(size_t col = 0; col <= width; ++col) {
            painter.drawLine(col*scale, 0, col*scale, height*scale);
        }
    }
*/
}

}


