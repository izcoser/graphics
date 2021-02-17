#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "bird.h"
#include "pipe.h"
#include "point.h"

#define WINDOW_SIZE_X 500
#define WINDOW_SIZE_Y 500

Bird bird = Bird(Point(0.1, 1), 0.03);
Pipe pipe = Pipe(Point(0.75, 0.6), Point(0.75, 0.8), 0.1);

void keyPress(unsigned char key, int x, int y){
    if(key == 'w'){
        bird.jump += 3;
        bird.jump_step = 0.025;
    }
    else if(key == 'q'){
        exit(0);
    }
}

void timer(int value){
    if(bird.jump){
        if(bird.center.y <= 1){
            bird.center.y += bird.jump_step;
            bird.jump_step -= 0.006;
        }
        bird.jump--;
    }
    else if(bird.center.y - bird.radius > 0){
        bird.center.y -= 0.005;
    }
    if(pipe.bottom.x + pipe.width > 0){
        pipe.bottom.x -= 0.005;
        pipe.top.x -= 0.005;
        pipe.update_points();
    }
    else{
        pipe.change();
    }

    if(bird.collision(pipe)){
        printf("Bird collided with pipe!\n");
        exit(0);
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 1);
}

void display(void){
    /* Limpar todos os pixels  */
    glClear (GL_COLOR_BUFFER_BIT);

    /* Desenhar o pássario. */
    bird.draw();

    /* Desenhar o tubo. */
    pipe.draw();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void init (void){
    /* Fundo azul. */
    glClearColor (66.0f/255.0f, 135.0f/255.0f, 245.0f/255.0f, 0.0);

    /* Inicializar sistema de visualização. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv){
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (WINDOW_SIZE_X, WINDOW_SIZE_Y); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello world");
    init ();
    glutDisplayFunc(display);

    glutKeyboardFunc(keyPress);
    //glutKeyboardUpFunc(keyUp);
    glutTimerFunc(16, timer, 1);
    //glutIdleFunc(idle);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
