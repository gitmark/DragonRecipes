/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef PAINTER_H
#define PAINTER_H
#include <string>
#include <DragonRecipes/Color.h>
#include <DragonRecipes/Brush.h>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Pen.h>
#include <DragonRecipes/Rect.h>
#include <DragonRecipes/Common.h>

namespace dragon {

class DRAGON_EXPORT Painter {
public:
    Painter () {}
    virtual ~Painter() {}

    virtual void setPen(Color color) = 0;
    virtual void setBrush(Brush brush) = 0;
    virtual void drawLine(Point p1, Point p2) = 0;
    virtual void drawEllipse(Point p1, Point p2) = 0;
    virtual void drawText(Point p, const std::string &text) = 0;
};

using PainterPtr = std::shared_ptr<Painter>;

}

#endif
