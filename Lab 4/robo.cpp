#include <stdio.h>

#include "robo.h"
#include <math.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
        glVertex3f( - (GLfloat)width / 2, 0, 0);
        glVertex3f(   (GLfloat)width / 2, 0, 0);
        glVertex3f(   (GLfloat)width / 2, (GLfloat)height, 0);
        glVertex3f( - (GLfloat)width / 2, (GLfloat)height, 0);
    glEnd();
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B){
    glPointSize(3);
    glColor3f(R, G, B);
    glBegin(GL_POINTS);
        for (int i = 0; i <= 20; i++){
            glVertex3f (radius * cos(i * 2 * M_PI / 20), radius * sin(i * 2 * M_PI / 20), 0);
        }
    glEnd();
}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B){
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(thetaWheel, 0, 0, 1);
    DesenhaCirc(radiusWheel, R, G, B);
    glPopMatrix();
}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(theta1, 0, 0, 1);
    this->DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1);
    glTranslatef(0, paddleHeight, 0);
    glRotatef(theta2, 0, 0, 1);
    this->DesenhaRect(paddleHeight, paddleWidth, 1, 1, 0);
    glTranslatef(0, paddleHeight, 0);
    glRotatef(theta3, 0, 0, 1);
    this->DesenhaRect(paddleHeight, paddleWidth, 0, 1, 0);
    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3){
    glPushMatrix();
    glTranslatef(x, y, 0);
    this->DesenhaRect(baseHeight, baseWidth, 1, 0, 0);
    this->DesenhaBraco(0, baseHeight, theta1, theta2, theta3);
    this->DesenhaRoda(-baseWidth / 2, 0, thetaWheel, 1, 1, 1);
    this->DesenhaRoda(baseWidth / 2, 0, thetaWheel, 1, 1, 1);
    glPopMatrix();
    
}

void Robo::RodaBraco1(GLfloat inc){
    this->gTheta1 += inc;

}

void Robo::RodaBraco2(GLfloat inc){
    this->gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc){
    this->gTheta3 += inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    gX += 20*dx;
    gThetaWheel -= 20*dx / 2 * M_PI;
}
/*
//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){

}*/

void RotatePoint(GLfloat &px, GLfloat &py, GLfloat angle, GLfloat ox, GLfloat oy){
    printf("Rotating %.2f, %.2f by %.2f, %.2f at angle %.2f\n", px, py, ox, oy, angle);
    GLfloat s = sin(angle);
    GLfloat c = cos(angle);

    px -= ox;
    py -= oy;

    GLfloat xnew = px * c - py * s;
    GLfloat ynew = px * s + py * c;

    px = xnew + ox;
    py = ynew + oy;
    printf("Result %.2f, %.2f\n", px, py);
}

Tiro* Robo::Atira(){
    GLfloat aux_x = gX;
    GLfloat aux_y = gY + baseHeight;

    GLfloat tiro_x = gX;
    GLfloat tiro_y = gY + baseHeight + paddleHeight;

    printf("First rotation:\n");
    RotatePoint(tiro_x, tiro_y, gTheta1 * 2 * M_PI / 360, aux_x, aux_y);
    aux_x = tiro_x;
    aux_y = tiro_y;

    tiro_y += paddleHeight;
    
    printf("Second rotation:\n");
    RotatePoint(tiro_x, tiro_y, (gTheta1 + gTheta2)* 2 * M_PI / 360, aux_x, aux_y);
    aux_x = tiro_x;
    aux_y = tiro_y;

    tiro_y += paddleHeight;

    printf("Third rotation:\n");
    RotatePoint(tiro_x, tiro_y, (gTheta1 + gTheta2 + gTheta3) * 2 * M_PI / 360, aux_x, aux_y);

    GLfloat gDirection = atan2((double)tiro_y - (double)aux_y, (double)tiro_x - (double)aux_x);

    Tiro* t = new Tiro(tiro_x, tiro_y, gDirection);
    return t;
}
