#include "fire.h"
#include "point.h"
#include <stdio.h>

Fire::Fire(Point pos, Point target){
    this->pos = this->startPos = pos;
    this->target = target;
    this->valid = 1;
}

void Fire::move(GLdouble timeDiff){
    pos += target * timeDiff * 0.01;
    printf("moved bullet to %.2f, %.2f, %.2f\n", pos.x, pos.y, pos.z);
    if(startPos.distance(pos) > 100){
        valid = 0;
    }
}