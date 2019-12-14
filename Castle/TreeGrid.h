/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TREE_GRID_H
#define TREE_GRID_H

#include <QWidget>
#include <QPainter>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>

using namespace dragon;

class Vec {
public:
    Vec(int x = 0, int y = 0) : x(x), y(y) {}
    int x;
    int y;
};

class Size {
public:
    Size(int width = 0, int height = 0) : width(width), height(height) {}
    int width;
    int height;
};

class NodeDim;

using NodeDimPtr = std::shared_ptr<NodeDim>;
NodeDimPtr newNodeDim(NodePtr node);

class NodeDim {
public:
    NodeDim() {}
    NodeDim(NodePtr node) : _node(node) {
        for (auto n : _node->children()) {
            addChild(newNodeDim(n));
        }

        Size s = size();
    }

    virtual ~NodeDim();

    void paint(QPainter &painter, int x, int y, float scale, float fontSize) {

        TokenPtr token = std::dynamic_pointer_cast<Token>(node()->symbol());
        std::string str;
        if (token)
            str = token->lexeme();
        else
            str = node()->symbol()->name();

        for (auto ch : children()) {
            Vec edge = ch->parentEdge();
            float offset = 0.5;
            painter.setPen(QColor(200,200,200));
            painter.drawLine((x + offset)*scale, (y + offset)*scale, (x+edge.x + offset)*scale, (y+edge.y + offset)*scale);
            ch->paint(painter, x + edge.x, y + edge.y, scale, fontSize);
        }

        painter.setBrush(QBrush(QColor(0,0,0)));
        painter.setPen(QColor(0,0,0));
        painter.drawEllipse(x*scale, y*scale, scale, scale);
        painter.setPen(QColor(200,200,200));
        painter.setBrush(QBrush(QColor(200,200,200)));
        painter.drawText(x*scale + (scale - fontSize)/2, y*scale + (fontSize + scale)/2, str.c_str());
    }

    Vec parentEdge() {
        return _parentEdge;
    }

    void setParentEdge(Vec edge) {
        _parentEdge = edge;
    }

    Size size() {

        if (_children.size() == 2) {

            NodeDimPtr &left = _children.front();
            NodeDimPtr &right = _children.back();

            Size leftSize = left->size();
            Size rightSize = right->size();

            left->setParentEdge(Vec(0, 1 + rightSize.height));
            right->setParentEdge(Vec(2, 0));

            int newWidth = std::max(leftSize.width, 2 + rightSize.width);
            int newHeight = leftSize.height + rightSize.height + 1;

            _size = Size(newWidth, newHeight);
            return _size;

        } else if (_children.size() == 1) {
            _children[0]->setParentEdge(Vec(1,1));
            _size = _children[0]->size();
        } else {
            _size = Size(1,1);
            return _size;
        }

        return _size;
    }

    void setSize(Size size) {
        _size = size;
    }

    NodePtr node() {
        return _node;
    }

    void setNode(NodePtr node) {
        _node = node;
    }

    std::vector<NodeDimPtr> &children() {
        return _children;
    }

    void addChild(NodeDimPtr nodeDim) {
        _children.push_back(nodeDim);
    }

private:
    Vec _parentEdge;
    Size _size;
    NodePtr _node;
    std::vector<NodeDimPtr> _children;
};

inline NodeDimPtr newNodeDim(NodePtr node) {
    return std::make_shared<NodeDim>(node);
}

class Cell {
public:
    Cell(const std::string &str = "", int usage = 0)
        : str(str), usage(usage) {}

    std::string str;
    int usage;
};

class TreeGrid : public QWidget
{
    Q_OBJECT

public:
    explicit TreeGrid(QWidget *parent = nullptr);
    void addToGrid(int row, int col, NodePtr node);
    void addToGrid(int row, int col, SymbolPtr symbol);
    void addToGrid(int row, int col, NodeDimPtr nodeDim);

protected:
    void paintEvent(QPaintEvent * /* event */);

signals:
public slots:

private:
    size_t width{10};
    size_t height{10};
    std::vector<std::vector<Cell>> grid;
};

#endif
