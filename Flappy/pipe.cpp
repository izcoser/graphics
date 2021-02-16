#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include "pipe.h"
#include "point.h"

Pipe::Pipe(Point _bottom, Point _top, float _width){
    bottom = _bottom;
    top = _top;
    width = _width;
}

void Pipe::draw(void){
    glColor3f (0.0, 1.0, 0.0);
    
    glBegin(GL_POLYGON);
        glVertex3f (bottom.x, 0.0, 0.0);
        glVertex3f (bottom.x + width, 0.0, 0.0);
        glVertex3f (bottom.x + width, bottom.y, 0.0);
        glVertex3f (bottom.x, bottom.y, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f (top.x, top.y, 0.0);
        glVertex3f (top.x + width, top.y, 0.0);
        glVertex3f (top.x + width, 1, 0.0);
        glVertex3f (top.x, 1, 0.0);
    glEnd();
}

void Pipe::change(void){
    bottom.x = top.x =  1;
    bottom.y = ((float) (rand() % 10)) / 10.0f;
    top.y = ((float) (rand() % 10)) / 10.0f;
    if(bottom.y > top.y){
        float aux = bottom.y;
        bottom.y = top.y;
        top.y = aux;
    }

    width = (((float) (rand() % 5)) + 1) / 10.0f;

    printf("New bottom.y, top.y, width: %.2f, %.2f, %.2f\n", bottom.y, top.y, width);
}