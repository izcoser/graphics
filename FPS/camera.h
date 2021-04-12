#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"

class Camera{
    public:
        Point pos;              // Camera position.
        Point target;           // At vector.
        Point up;               // Up vector.
        
        //Point direction;        // Not the best chosen vector name, since it is actually pointing in the reverse direction of what it is targeting.
        //Point right;            // Right vector.

        Camera();
        Camera(Point pos, Point target, Point up);
};

#endif