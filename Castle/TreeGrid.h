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
    Vec(float x = 0, float y = 0) : x(x), y(y) {}
    float x;
    float y;
};

class Size {
public:
    Size(float width = 0, float height = 0) : width(width), height(height) {}
    float width;
    float height;
};

class NodeDim;

using NodeDimPtr = std::shared_ptr<NodeDim>;
NodeDimPtr newNodeDim(NodePtr node);

class TextCanvas {
public:
    TextCanvas(int rows = 200, int cols = 80) : rows(rows), cols(cols) {
        buf.resize(static_cast<size_t>(rows));

        for (auto &row : buf)
            row.resize(static_cast<size_t>(cols + 1)); // add one for null char
    }

    void drawLine(Point p1, Point p2) {
/*
        p1.x *= 2.0f;
        p1.y *= 2.0f;
        p2.x *= 2.0f;
        p2.y *= 2.0f;
*/
        int x1 = std::round(p1.x);
        int y1 = std::round(p1.y);

        int x2 = std::round(p2.x);
        int y2 = std::round(p2.y);

        int dx = x2 - x1;
        int dy = y2 - y1;

        if (dx > 0)
            dx = 1;
        else if (dx < 0)
            dx = -1;

        if (dy > 0)
            dy = 1;
        else if (dy < 0)
            dy = -1;

        int x = x1;
        int y = y1;

        std::string ch = ".";

        if (dx == dy)
            ch = "\\";
        else if (dx == -dy)
            ch = "/";
        else if (dx == 0)
            ch = "|";
        else
            ch = "-";
        x -= dx;
        y -= dy;
        do {
            x += dx;
            y += dy;
            drawText(Point(x,y), ch);
        } while(x != x2 && y != y2);


    }

    void drawText(Point point, const std::string &text) {

  //      point.x *= 2.0f;
   //     point.y *= 2.0f;
        if (text.empty())
            return;

        int x1 = static_cast<int>(std::rint(point.x));
        int y1 = static_cast<int>(std::rint(point.y));

        int startX = x1 - text.size()/2;
        int stopX = startX + text.size();

        int j = 0;
        for (int x = startX; x < stopX; ++x)
            buf[y1][x] = text[j++];

        for (int i = 0; i < x1; ++i) {
            if (buf[y1][i] == 0)
                buf[y1][i] = ' ';
        }

    }

    std::string str() {
        std::stringstream ss;
        for (int row = 0; row < rows; ++row) {
            if (buf[row][0] != 0)
               ss << buf[row].data() << "\n";
        }

        return ss.str();
    }

private:
    int rows;
    int cols;
    std::vector<std::vector<char>> buf;
};

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

    std::string textTree() {
        return _canvas->str();
    }
    int toInt(double d) {
        return static_cast<int>(rint(d));
    }

    int toInt(float f) {
        return static_cast<int>(rint(f));
    }

    void drawLine(Point p1, Point p2) {
        Point p5 = rotate(_center0, _angle, p1);
        Point p6 = rotate(_center0, _angle, p2);
        _canvas->drawLine(p5,p6);

        p1.x *= _scale;
        p1.y *= _scale;

        p2.x *= _scale;
        p2.y *= _scale;

        Point p3 = rotate(_center, _angle, p1);
        Point p4 = rotate(_center, _angle, p2);

        _painter->drawLine(toInt(p3.x), toInt(p3.y), toInt(p4.x), toInt(p4.y));
    }

    void drawText(const Point &point, const std::string &text) {
        Point p1(point.x*_scale, point.y*_scale);
        Point p3 = rotate(_center, _angle, p1);
        Point p4 = rotate(_center0, _angle, point);

        _canvas->drawText(p4, text);

        _painter->setBrush(QBrush(QColor(0,0,0)));
        _painter->setPen(QColor(0,0,0));
        float eScale = 2.5f;
        _painter->drawEllipse(toInt(p3.x - _fontSize*eScale/2), toInt(p3.y - _fontSize*eScale/2), toInt(_fontSize*eScale), toInt(_fontSize*eScale));
        _painter->setPen(QColor(200,200,200));
        _painter->drawText(toInt(p3.x - _fontSize/2), toInt(p3.y + _fontSize/2), text.c_str());
    }

    void paint(QPainter &painter, TextCanvas &canvas, Point center, Point point, float scale, float fontSize) {
        _painter = &painter;
        _scale = scale;
        _angle = -static_cast<float>(M_PI)/4.0f;
        _center0 = center;
        _center = Point(toInt((_center0.x + 0.5) * _scale), toInt((_center0.y + 0.5) * _scale));
        _fontSize = fontSize;
        _canvas = &canvas;

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
            drawLine(Point(x, y), Point(x+edge.x, y+edge.y));
            ch->paint(painter, canvas, _center0, Point((x + edge.x), (y + edge.y)), scale, fontSize);
        }

        painter.setBrush(QBrush(QColor(0,0,0)));
        painter.setPen(QColor(0,0,0));
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

            left->setParentEdge(Vec(0, 2.828f + rightSize.height));
            right->setParentEdge(Vec(2.828f, 0));

            int newWidth = std::max(leftSize.width, 2.828f + rightSize.width);
            int newHeight = leftSize.height + rightSize.height + 2.828f;

            _size = Size(newWidth, newHeight);
            } else {
                left->setParentEdge(Vec(0, 2.828f));
                right->setParentEdge(Vec(2.828f + leftSize.width, 0));

                float newHeight = std::max(leftSize.height + 2.828f, rightSize.height);
                float newWidth = leftSize.width + rightSize.width + 2.828f;

                _size = Size(newWidth, newHeight);
            }
            return _size;

        } else if (_children.size() == 1) {
            _children[0]->setParentEdge(Vec(1.414f,1.414f));
            _size = _children[0]->size();
        } else {
            _size = Size(0.0f,0.0f);
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
    Point _center0;
    float _fontSize{24};
    TextCanvas *_canvas;

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
    TextCanvas textCanvas;
};

#endif
