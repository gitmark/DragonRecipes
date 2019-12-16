/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef QT_PAINTER_H
#define QT_PAINTER_H

#include <QPainter>
#include <DragonRecipes/Painter.h>
#include <DragonRecipes/Color.h>
#include <DragonRecipes/Brush.h>
#include <DragonRecipes/Pen.h>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Transform.h>

using namespace dragon;

namespace castle {

class QtPainter : public dragon::Painter {
public:
    QtPainter(QPainter *painter) : painter(painter) {}

    virtual ~QtPainter();
    virtual void setPen(Color c) {
        painter->setPen(QColor(c.red, c.green, c.blue, c.alpha));
    }

    virtual void setBrush(Brush b) {
        painter->setBrush(QBrush(QColor(b.color.red, b.color.green, b.color.blue, b.color.alpha)));
    }
    virtual void drawLine(Point p1, Point p2) {
        painter->drawLine(toInt(p1.x), toInt(p1.y), toInt(p2.x), toInt(p2.y));
    }

    virtual void drawEllipse(Point p1, Point p2) {
        painter->drawEllipse(toInt(p1.x), toInt(p1.y), toInt(p2.x - p1.x), toInt(p2.y - p1.y));
    }
    virtual void drawText(Point p, const std::string &text) {
        painter->drawText(toInt(p.x), toInt(p.y), text.c_str());
    }

    QPainter *painter;
};

inline PainterPtr newPainter(QPainter &painter) {
    return std::make_shared<QtPainter>(&painter);
}


}

#endif

