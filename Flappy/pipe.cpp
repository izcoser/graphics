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
    else if(top.y - bottom.y <= 0.20){
        bottom.y -= 0.2;
    }

    width = (((float) (rand() % 50)) + 10) / 100.0f;
    this->update_points();

    printf("New bottom.y, top.y, width: %.2f, %.2f, %.2f\n", bottom.y, top.y, width);
}

void Pipe::update_points(void){
    points[0].x = bottom.x;                 points[0].y = 0;
    points[1].x = bottom.x;                 points[1].y = bottom.y / 2;
    points[2].x = bottom.x + width / 2;     points[2].y = bottom.y;
    points[3].x = bottom.x + width;         points[3].y = bottom.y;
    points[4].x = bottom.x + width;         points[4].y = bottom.y / 2;
    points[5].x = bottom.x + width;         points[5].y = 0;

    points[6].x = top.x;                    points[6].y = 1;
    points[7].x = top.x;                    points[7].y = (top.y + 1) / 2;
    points[8].x = top.x + width / 2;        points[8].y = top.y;
    points[9].x = top.x + width;            points[9].y = top.y;
    points[10].x = top.x + width;           points[10].y = (top.y + 1) / 2;
    points[11].x = top.x + width;           points[11].y = 1;
}