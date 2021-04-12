#include "camera.h"
#include "point.h"

Camera::Camera(){
    pos = Point(0, 0, 0);
    target = Point(0, 0, -1);
    up = Point(0, 1, 0);
}

Camera::Camera(Point pos, Point target, Point up){
    this->pos = pos;
    this->target = target;
    this->up = up;
}