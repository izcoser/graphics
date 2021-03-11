#include "point.h"
#include "math.h"

Point::Point(GLfloat _x, GLfloat _y){
    x = _x;
    y = _y;
}

GLfloat Point::distance(Point a){
    return sqrt(pow(x - a.x, 2) + pow(y - a.y, 2)); 
}

Point Point::sum(Point a){
    return Point(x + a.x, y + a.y);
}

Point Point::rotate(GLfloat angle){
    angle *= M_PI / 180;
    GLfloat s = sin(angle);
    GLfloat c = cos(angle);
    return Point(x * c - y * s, x * s + y * c);
}