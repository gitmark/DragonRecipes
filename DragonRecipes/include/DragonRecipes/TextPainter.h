/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TEXT_PAINTER_H
#define TEXT_PAINTER_H

#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Transform.h>
#include <DragonRecipes/Painter.h>
#include <DragonRecipes/TextCanvas.h>
#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT TextPainter : public dragon::Painter {
public:
    TextPainter(TextCanvas *painter) : painter(painter) {}

    virtual ~TextPainter();
    virtual void setPen(Color c) {
//        painter->setPen(QColor(c.red, c.green, c.blue, c.alpha));
    }

    virtual void setBrush(Brush b) {
  //      painter->setBrush(QBrush(QColor(b.color.red, b.color.green, b.color.blue, b.color.alpha)));
    }
    virtual void drawLine(Point p1, Point p2) {
        painter->drawLine(p1, p2);
    }

    virtual void drawEllipse(Point p1, Point p2) {
//        painter->drawEllipse(toInt(p1.x), toInt(p1.y), toInt(p2.x - p1.x), toInt(p2.y - p1.y));
    }
    virtual void drawText(Point p, const std::string &text) {
        painter->drawText(p, text);
    }

    TextCanvas *painter;
};

inline PainterPtr newPainter(TextCanvas &painter) {
    return std::make_shared<TextPainter>(&painter);
}

}

#endif
