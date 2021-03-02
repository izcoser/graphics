#ifndef POINT_H
#define POINT_H

class Point{
    public:
        float x;
        float y;
        
        float distance(Point a);

        Point(float _x, float _y);
        Point() = default;
};

#endif