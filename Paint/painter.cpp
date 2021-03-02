#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include "painter.h"

Painter::Painter(void){
    size = 3.0;
    drawing_line = 0;
    drawing_circle = 0;
    picking_color = 0;

    red = Point(0.9, 0.9);
    green = Point(1.0, 0.9);
    blue = Point(0.95, 1.0);
}

void Painter::get_new_color(Point p){
    float mBClick = blue.x != p.x ? (blue.y - p.y) / (blue.x - p.x) : 100.0;
    float bBClick = blue.y - mBClick * blue.x;

    float mRG = red.x != green.x ? (red.y - green.y) / (red.x - green.x) : 100.0;
    float bRG = red.y - mRG * red.x;

    float proj_x = (bRG - bBClick) / (mBClick - mRG);
    float proj_y = mBClick * proj_x + bBClick;
    Point proj = Point(proj_x, proj_y);

    float proportionB = p.distance(blue) / proj.distance(blue);
    float distanceRG = green.distance(red);
    float gB = 1.0 - proportionB;
    float gR = (1.0 - ( proj.distance(red) / distanceRG )) * proportionB;
    float gG = (1.0 - ( proj.distance(green) / distanceRG )) * proportionB;

    printf("Picked color %.2f, %.2f, %.2f.\n", gR, gG, gB);
    glColor3f(gR, gG, gB);
    picking_color--;

}

void Painter::remove_color_picker(void){
    glBegin(GL_POLYGON);
        glColor3f (1.0, 1.0, 1.0);
        glVertex3f (red.x, red.y, 0.0);
        glVertex3f (green.x, green.y, 0.0);
        glVertex3f (blue.x, blue.y, 0.0);
    glEnd();
}

void Painter::display_color_picker(void){
    glBegin(GL_POLYGON);
        glColor3f (1.0, 0.0, 0.0);
        glVertex3f (red.x, red.y, 0.0);
        glColor3f (0.0, 1.0, 0.0);
        glVertex3f (green.x, green.y, 0.0);
        glColor3f (0.0, 0.0, 1.0);
        glVertex3f (blue.x, blue.y, 0.0);
    glEnd();
}