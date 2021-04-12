#ifndef ENEMY_H
#define ENEMY_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"
#include "objloader.h"
#include "camera.h"
#include <list>

class Enemy{
    public:
        Point pos;              // Camera position.
        Point target;           // At vector.
        Mesh mesh;
        GLfloat angle;
        Enemy() = default;
        Enemy(Point pos, Mesh mesh);

        void draw(void);
        void moveTowardsPlayer(Point pos, GLdouble timeDiff, list<Enemy> enemies);
        void lookAt(Point pos);
};

#endif