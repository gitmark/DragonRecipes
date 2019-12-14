/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TREE_GRID_H
#define TREE_GRID_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <QWidget>
#include <QPainter>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/Symbol.h>
#include <DragonRecipes/Token.h>

using namespace dragon;

class Point {
public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    float x;
    float y;
};

inline Point rotate(Point center, float angle, Point point) {

    float x = 0;
    float y = 0;

    float x1 = point.x - center.x;
    float y1 = point.y - center.y;

    float c = std::cos(-angle);
    float s = std::sin(-angle);
    x = c*x1 - s*y1;
    y = s*x1 + c*y1;

    return Point(center.x + x, center.y + y);
}

inline Point scale(Point center, float s, Point point) {

    float x = 0;
    float y = 0;

    float x1 = point.x - center.x;
    float y1 = point.y - center.y;

    x = s*x1;
    y = s*y1;

    return Point(center.x + x, center.y + y);
}


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

    int toInt(double d) {
        return static_cast<int>(rint(d));
    }

    int toInt(float f) {
        return static_cast<int>(rint(f));
    }

    void drawLine(float x1, float y1, float x2, float y2) {
        Point p1(x1,y1);
        Point p2(x2,y2);

        Point p3 = rotate(_center, _angle, p1);
        Point p4 = rotate(_center, _angle, p2);
        _painter->drawLine(toInt(p3.x), toInt(p3.y), toInt(p4.x), toInt(p4.y));
    }

    void drawText(const Point &point, const std::string &text) {
//        Point p1(point.x*_scale + (_scale - _fontSize)/2, point.y*_scale + (_fontSize + _scale)/2);
        Point p1(point.x*_scale, point.y*_scale);
        Point p3 = rotate(_center, _angle, p1);
        _painter->setBrush(QBrush(QColor(0,0,0)));
        _painter->setPen(QColor(0,0,0));
        float eScale = 2.5f;
        _painter->drawEllipse(toInt(p3.x - _fontSize*eScale/2), toInt(p3.y - _fontSize*eScale/2), toInt(_fontSize*eScale), toInt(_fontSize*eScale));
        _painter->setPen(QColor(200,200,200));
        _painter->drawText(toInt(p3.x - _fontSize/2), toInt(p3.y + _fontSize/2), text.c_str());
    }

    void paint(QPainter &painter, Point center, Point point, float scale, float fontSize) {
        _painter = &painter;
        _scale = scale;
        _angle = -static_cast<float>(M_PI)/4.0f;
        _center = center;
        _fontSize = fontSize;

        float x = point.x;
        float y = point.y;

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
//            drawLine((x + offset)*scale, (y + offset)*scale, (x+edge.x + offset)*scale, (y+edge.y + offset)*scale);
            drawLine((x)*scale, (y)*scale, (x+edge.x)*scale, (y+edge.y)*scale);
            ch->paint(painter, _center, Point((x + edge.x), (y + edge.y)), scale, fontSize);
        }

        painter.setBrush(QBrush(QColor(0,0,0)));
        painter.setPen(QColor(0,0,0));
//        painter.drawEllipse(toInt(x*scale), toInt(y*scale), toInt(scale), toInt(scale));
        painter.setPen(QColor(200,200,200));
        painter.setBrush(QBrush(QColor(200,200,200)));
        drawText(point, str);
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

            if (leftSize.width > rightSize.height*0.95) {

            left->setParentEdge(Vec(0, 1 + rightSize.height));
            right->setParentEdge(Vec(2, 0));

            int newWidth = std::max(leftSize.width, 2 + rightSize.width);
            int newHeight = leftSize.height + rightSize.height + 1;

            _size = Size(newWidth, newHeight);
            } else {
                left->setParentEdge(Vec(0, 2));
                right->setParentEdge(Vec(1 + leftSize.width, 0));

                int newHeight = std::max(leftSize.height, 2 + rightSize.height);
                int newWidth = leftSize.width + rightSize.width + 1;

                _size = Size(newWidth, newHeight);
            }
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
    float _angle{-static_cast<float>(M_PI)/4.0f};
    float _scale{20};
    QPainter *_painter{nullptr};
    Point _center;
    float _fontSize{24};
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
    void addTree(NodePtr node);

protected:
    void paintEvent(QPaintEvent * /* event */);

signals:
public slots:

private:
    size_t width{10};
    size_t height{10};
    std::vector<std::vector<Cell>> grid;
    std::vector<NodeDimPtr> trees;
};

#endif
