#include "alvo.h"
#include <math.h>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for(int i = 0; i <= 20; i++){
            glVertex3f (radius * cos(i * 2 * M_PI / 20), radius * sin(i * 2 * M_PI / 20), 0);
        }
    glEnd();
}

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    if(gColor == 0){
        DesenhaCirc(radiusAlvo, 1, 0, 0);
    }
    else if(gColor == 1){
        DesenhaCirc(radiusAlvo, 0, 1, 0);
    }
    else if(gColor == 2){
        DesenhaCirc(radiusAlvo, 0, 0, 1);
    }
    
    
    glPopMatrix();
}

void Alvo::Recria(GLfloat x, GLfloat y){
    gX = x;
    gY = y;
    gColor += 1;
    gColor %= 3;
}

bool Alvo::Atingido(Tiro *tiro){
    GLfloat tiro_x;
    GLfloat tiro_y;
    tiro->GetPos(tiro_x, tiro_y);
    return sqrt(pow(tiro_x - gX, 2) + pow(tiro_y - gY, 2)) <= radiusAlvo;
}
