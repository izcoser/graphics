#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>

#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

// Pipe x coordinate.
float pipe_x = 0.75;

// Pipe width.
float pipe_width = 0.1;

// Pipe y1 and y2 coordinates.
float pipe_y1 = 0.6;
float pipe_y2 = 0.8;

// Jump
int jump = 0;

// Bird size.
float bird_size = 0.03;

// Centro do quadrado. 
float bird_x = 0.1;
float bird_y = 1;

void keyPress(unsigned char key, int x, int y){
    if(key == 'w'){
        jump += 3;
    }
    else if(key == 'q'){
        exit(0);
    }
}

void timer(int value){
    if(jump){
        bird_y += 0.025;
        jump--;
    }
    else if(bird_y - bird_size > 0){
        bird_y -= 0.005;
    }
    if(pipe_x + pipe_width > 0){
        pipe_x -= 0.005;
    }
    else{
        pipe_x = 1;
        pipe_y1 = ((float) (rand() % 10)) / 10.0f;
        pipe_y2 = ((float) (rand() % 10)) / 10.0f;
        if(pipe_y1 > pipe_y2){
            float aux = pipe_y1;
            pipe_y1 = pipe_y2;
            pipe_y2 = aux;
        }
        printf("New pipe_y1, pipe_y2: %.2f, %.2f\n", pipe_y1, pipe_y2);
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 1);
}

void display(void){
    /* Limpar todos os pixels  */
    glClear (GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f (128, 128, 0.4);
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
        glVertex3f (bird_x - bird_size, bird_y - bird_size, 0.0);
        glVertex3f (bird_x + bird_size, bird_y - bird_size, 0.0);
        glVertex3f (bird_x + bird_size, bird_y + bird_size, 0.0);
        glVertex3f (bird_x - bird_size, bird_y + bird_size, 0.0);
    glEnd();

    glColor3f (0.0, 128.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f (pipe_x, 0.0, 0.0);
        glVertex3f (pipe_x + pipe_width, 0.0, 0.0);
        glVertex3f (pipe_x + pipe_width, pipe_y1, 0.0);
        glVertex3f (pipe_x, pipe_y1, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex3f (pipe_x, pipe_y2, 0.0);
        glVertex3f (pipe_x + pipe_width, pipe_y2, 0.0);
        glVertex3f (pipe_x + pipe_width, 1, 0.0);
        glVertex3f (pipe_x, 1, 0.0);
    glEnd();


    /* Desenhar no frame buffer! */
    glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void init (void){
    /* selecionar cor de fundo (preto) */
    glClearColor (66.0f/255.0f, 135.0f/255.0f, 245.0f/255.0f, 0.0);

    /* inicializar sistema de visualizacao */
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
