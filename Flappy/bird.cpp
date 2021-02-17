#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "bird.h"
#include "math.h"
#include "pipe.h"

Bird::Bird(Point _center, float _radius){
    center = _center;
    radius = _radius;
}

void Bird::draw(void){
    glColor3f (128, 128, 0.4);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(center.x, center.y, 0);
    for (int i = 0; i <= 20; i++)   {
        glVertex3f (
            (center.x + (radius * cos(i * 2 * M_PI / 20))), (center.y + (radius * sin(i * 2 * M_PI / 20))), 0);
    }
    glEnd();
}

int Bird::collision(Pipe pipe){
    for(int i = 0; i < PIPE_POINTS; i++){
        if(center.distance(pipe.points[i]) < radius){
            return 1;
        }
    }
    return center.distance(pipe.bottom) < radius || center.distance(pipe.top) < radius;
}