#ifndef PAINTER_H
#define PAINTER_H

#include "point.h"

class Painter{
    public:
        float size;
        Point line_start;
        Point circle_center;
        int drawing_line;
        int drawing_circle;
        int picking_color;

        /* Color Picker Triangle */
        Point red;
        Point green;
        Point blue;

        Painter(void);
        void get_new_color(Point p);
        void remove_color_picker(void);
        void display_color_picker(void);
};

#endif