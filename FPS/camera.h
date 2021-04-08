#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"

class Camera{
    public:
        Point p;        // Camera position.
        Point lookat;   // At vector.
        Point up;       // Up vector.

        Camera();
        Camera(Point p, Point lookat, Point up);
};

#endif