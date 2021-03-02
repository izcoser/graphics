#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "point.h"
#include "painter.h"

#define WIDTH 1280
#define HEIGHT 720

Painter painter = Painter();

void display(void)
{
   /* Desenhar no frame buffer! */
   glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void init (void) 
{
    /* selecionar cor de fundo (branco) */
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(painter.size);
    glLineWidth(painter.size);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void motion(int x, int y){
    y = HEIGHT - y;
    GLfloat fX = (GLfloat)x/WIDTH;
    GLfloat fY = (GLfloat)y/HEIGHT;

    glBegin(GL_POINTS);
    glVertex3f(fX, fY, 0.0);
    glEnd();

    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    y = HEIGHT - y;
    Point p = Point((GLfloat)x/WIDTH, (GLfloat)y/HEIGHT);

    if(button == GLUT_LEFT_BUTTON && state == 0){
        if(painter.drawing_line == 2){
            painter.line_start = Point(p.x, p.y);
            painter.drawing_line--;
            printf("Begin drawing a line.\n");

        }else if(painter.drawing_line == 1){
            painter.drawing_line--;
            glBegin(GL_LINES);
                glVertex3f(painter.line_start.x, painter.line_start.y, 0.0);
                glVertex3f(p.x, p.y, 0.0);
            glEnd();
            printf("Finished drawing line.\n");
        }

        else if(painter.drawing_circle == 2){
            painter.circle_center = Point(p.x, p.y);
            painter.drawing_circle--;
            printf("Begin drawing a circle.\n");
        }
        
        else if(painter.drawing_circle == 1){
            float radius = painter.circle_center.distance(p);
            painter.drawing_circle--;
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(painter.circle_center.x, painter.circle_center.y, 0);
                for (int i = 0; i <= 20; i++)   {
                    glVertex3f (
                        (painter.circle_center.x + (radius * cos(i * 2 * M_PI / 20))), (painter.circle_center.y + (radius * sin(i * 2 * M_PI / 20))), 0);
                }
            printf("Finished drawing line.\n");
        }else if(painter.picking_color){
            painter.remove_color_picker();
            painter.get_new_color(p);            
        }

        glBegin(GL_POINTS);
            glVertex3f((GLfloat)p.x, (GLfloat)p.y, 0.0);
        glEnd();
        printf("Drawing point at %.2f, %.2f\n", p.x, p.y);
    }
        glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y){
    switch(key){
        case '-':
            if(painter.size > 1.0){
                painter.size -= 1.0;
                glPointSize(painter.size);
                glLineWidth(painter.size);
                printf("Point size changed to %.2f.\n", painter.size);
            }
            break;
        case '+':
            if(painter.size < 100.0){
                painter.size += 1.0;
                glPointSize(painter.size);
                glLineWidth(painter.size);
                printf("Point size changed to %.2f.\n", painter.size);
            }
            break;
        case 'e':
                glColor3f(1.0, 1.0, 1.0);
                break;
        case 'p':
                glColor3f(0.0, 0.0, 0.0);
                break;
        case 'l':
                painter.drawing_line = 2;
                printf("Begin drawing a line.\n");
                break;
        case 'c':
                painter.drawing_circle = 2;
                printf("Begin drawing a circle.\n");
                break;
        case 'C':
                painter.picking_color = 1;
                printf("Drawing color triangle.\n");
                painter.display_color_picker();

                glutPostRedisplay();

    }
}

void reshape (int w, int h){
    /*glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
    */
}
    
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (WIDTH, HEIGHT); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Paint");
    init ();
    glutDisplayFunc(display); 
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyPress);
    //glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
