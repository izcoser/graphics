#ifndef PIPE_H
#define PIPE_H

#include "point.h"

class Pipe{
    public:
        Point bottom;
        Point top;
        float width;

        Pipe(Point _bottom, Point _top, float _width);
        void draw(void);
        void change(void);
};

#endif