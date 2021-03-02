#include "tiro.h"
#include <math.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for(int i = 0; i <= 20; i++){
            glVertex3f (radius * cos(i * 2 * M_PI / 20), radius * sin(i * 2 * M_PI / 20), 0);
        }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y){
    glPushMatrix();
    glTranslatef(x, y, 0);
    DesenhaCirc(radiusTiro, 1, 1, 1);
    glPopMatrix();
}

void Tiro::Move(GLdouble timeDifference){
    gX += cos(gDirectionAng) * gVel * timeDifference;
    gY += sin(gDirectionAng) * gVel * timeDifference;
}

bool Tiro::Valido(){
    return sqrt(pow(gX - gXInit, 2) + pow(gY - gYInit, 2)) < DISTANCIA_MAX;
}
