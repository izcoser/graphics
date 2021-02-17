#ifndef PIPE_H
#define PIPE_H
#define PIPE_POINTS 12

#include "point.h"

class Pipe{
    public:
        Point bottom;
        Point top;
        float width;

        Point points[PIPE_POINTS];

        Pipe(Point _bottom, Point _top, float _width);
        void draw(void);
        void change(void);
        void update_points(void);
};

#endif