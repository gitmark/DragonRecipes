/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef NODE_DIM_H
#define NODE_DIM_H

#include <cmath>
#include <memory>
#include <DragonRecipes/Node.h>
#include <DragonRecipes/Size.h>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Painter.h>
#include <DragonRecipes/Rect.h>
#include <DragonRecipes/Transform.h>
#include <DragonRecipes/TextCanvas.h>
#include <DragonRecipes/Brush.h>
#include <DragonRecipes/Pen.h>
#include <DragonRecipes/Color.h>
#include <DragonRecipes/Token.h>
#include <DragonRecipes/Common.h>


namespace dragon {

using Vec = Point;

class NodeDim;

using NodeDimPtr = std::shared_ptr<NodeDim>;
NodeDimPtr newNodeDim(NodePtr node);

class DRAGON_EXPORT NodeDim {
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

    void drawLine(Point p1, Point p2) {
//        Point p5 = rotate(_center0, _angle, p1);
//        Point p6 = rotate(_center0, _angle, p2);

        p1.x *= _scale;
        p1.y *= _scale;

        p2.x *= _scale;
        p2.y *= _scale;

        Point p3 = rotate(_center, _angle, p1);
        Point p4 = rotate(_center, _angle, p2);

        _painter->drawLine(p3, p4);
    }

    void drawLine2(Point p1, Point p2) {
        Point p5 = rotate(_center0, _angle, p1);
        Point p6 = rotate(_center0, _angle, p2);
        _canvas->drawLine(p5,p6);
    }


    void drawText(const Point &point, const std::string &text) {
        Point p1(point.x*_scale, point.y*_scale);
        Point p3 = rotate(_center, _angle, p1);
        Point p4 = rotate(_center0, _angle, point);

        _painter->setBrush(Brush(Color(0,0,0)));
        _painter->setPen(Color(0,0,0));
 //       float eScale = 2.5f;
        _painter->drawEllipse(Point(p3.x - _fontSize, p3.y - _fontSize), Point(p3.x + _fontSize, p3.y + _fontSize));
        _painter->setPen(Color(200,200,200));

        int x = toInt(p4.x);
        int y = toInt(p4.y);
        int x1 = x - static_cast<int>(text.size()/2);
        int x2 = x1 + static_cast<int>(text.size() - 1);

        if (x1 < _canvasRect->x1)
            _canvasRect->x1 = x1;

        if (x2 > _canvasRect->x2)
            _canvasRect->x2 = x2;

        if (y < _canvasRect->y1)
            _canvasRect->y1 = y;

        if (y > _canvasRect->y2)
            _canvasRect->y2 = y;

        int x3 = toInt(p3.x);
        int y3 = toInt(p3.y);

        _painter->drawText(Point(x3 - static_cast<int>(text.size()/2) - _fontSize/2, y3 + _fontSize/2), text.c_str());
    }

    void drawText2(const Point &point, const std::string &text) {
        Point p4 = rotate(_center0, _angle, point);
        _canvas->drawText(p4, text);
    }


    void paint(PainterPtr painter, Rect &canvasRect, Point center, Point point, float scale, float fontSize) {
        _painter = painter;
        _scale = scale;
        _angle = -static_cast<float>(M_PI)/4.0f;
        _center0 = center;
        _center = Point(toInt((_center0.x + 0.5f) * _scale), toInt((_center0.y + 0.5f) * _scale));
        _fontSize = fontSize;
        _canvasRect = &canvasRect;

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
//            float offset = 0.5;
            painter->setPen(Color(200,200,200));
            drawLine(Point(x, y), Point(x+edge.x, y+edge.y));
            ch->paint(painter, *_canvasRect, _center0, Point((x + edge.x), (y + edge.y)), scale, fontSize);
        }

        painter->setBrush(Brush(Color(0,0,0)));
        painter->setPen(Color(0,0,0));
        painter->setPen(Color(200,200,200));
        painter->setBrush(Brush(Color(200,200,200)));
        drawText(point, str);
    }

    void paint(TextCanvas &canvas, Rect &canvasRect, Point center, Point point, float scale, float fontSize) {
        _scale = scale;
        _angle = -static_cast<float>(M_PI)/4.0f;
        _center0 = center;
        _center = Point(toInt((_center0.x + 0.5f) * _scale), toInt((_center0.y + 0.5f) * _scale));
        _fontSize = fontSize;
        _canvas = &canvas;
        _canvasRect = &canvasRect;

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
//            float offset = 0.5;
            drawLine2(Point(x, y), Point(x+edge.x, y+edge.y));
            ch->paint(canvas, *_canvasRect, _center0, Point((x + edge.x), (y + edge.y)), scale, fontSize);
        }

        drawText2(point, str);
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

            if (leftSize.width > rightSize.height*0.95f) {

                left->setParentEdge(Vec(0, 2.828f + rightSize.height));
                right->setParentEdge(Vec(2.828f, 0));

                float newWidth = std::max(leftSize.width, 2.828f + rightSize.width);
                float newHeight = leftSize.height + rightSize.height + 2.828f;

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
    PainterPtr _painter;
    Point _center;
    Point _center0;
    float _fontSize{24};
    TextCanvas *_canvas;
    Rect *_canvasRect;

};

inline NodeDimPtr newNodeDim(NodePtr node) {
    return std::make_shared<NodeDim>(node);
}

}

#endif

