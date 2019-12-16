/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TREE_GRID_H
#define TREE_GRID_H

#define _USE_MATH_DEFINES

#include <sstream>
#include <cmath>
#include <QWidget>
#include <QPainter>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/NodeDim.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Cell.h>
#include <DragonRecipes/TextCanvas.h>

using namespace dragon;

namespace castle {
using Vec = Point;

class TreeGrid : public QWidget
{
    Q_OBJECT

public:
    explicit TreeGrid(QWidget *parent = nullptr);
    void addToGrid(int row, int col, NodePtr node);
    void addToGrid(int row, int col, SymbolPtr symbol);
    void addToGrid(int row, int col, NodeDimPtr nodeDim);
    void addTree(NodePtr node);
    std::string textTree();

protected:
    void paintEvent(QPaintEvent * /* event */);

signals:
public slots:

private:
    size_t width{10};
    size_t height{10};
    std::vector<std::vector<Cell>> grid;
    std::vector<NodeDimPtr> trees;
    std::string _treeText;
//    TextCanvas textCanvas;
};

}

#endif
