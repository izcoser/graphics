#ifndef BIRD_H
#define BIRD_H

#include "point.h"
#include "pipe.h"

class Bird{
    public:
        Point center;
        float radius;
        int jump;
        float jump_step;

        Bird(Point _center, float _radius);
        void draw(void);
        int collision(Pipe pipe);
};

#endif