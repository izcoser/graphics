#include "camera.h"
#include "point.h"

Camera::Camera(){
    p = Point(0, 0, 0);
    lookat = Point(0, 0, -1);
    up = Point(0, 1, 0);
}

Camera::Camera(Point p, Point lookat, Point up){
    this->p = p;
    this->lookat = lookat;
    this->up = up;
}