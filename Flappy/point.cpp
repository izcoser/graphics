#include "point.h"
#include "math.h"

Point::Point(float _x, float _y){
    x = _x;
    y = _y;
}

float Point::distance(Point a){
    return sqrt(pow(x - a.x, 2) + pow(y - a.y, 2)); 
}