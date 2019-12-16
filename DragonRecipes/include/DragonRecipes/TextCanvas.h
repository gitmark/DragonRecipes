/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef TEXT_CANVAS_H
#define TEXT_CANVAS_H

#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <DragonRecipes/Point.h>
#include <DragonRecipes/Transform.h>

namespace dragon {

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
        int x1 = toInt(p1.x);
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
            if (buf[y1][i] == 0 || buf[y1][i] == '\n')
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

}

#endif
