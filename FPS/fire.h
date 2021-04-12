#ifndef FIRE_H
#define FIRE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"

class Fire{
    public:
        Point pos;              // Fire position.
        Point target;
        Point startPos;
        int valid;

        Fire() = default;
        Fire(Point pos, Point target);
        void move(GLdouble timeDiff);
};

#endif