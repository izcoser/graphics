#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

// Centro do quadrado. 
float gX = 0.5;
float gY = 0.5;

// Salva coordenadas ao clicar com o mouse no quadrado para fazer arrasto.
float dragX = -1;
float dragY = -1;

// Se clicou dentro do quadrado, inside = 1, caso contrário inside = 0.
int inside = 0;

int keyStatus[256] = {0};

void keyPress(unsigned char key, int x, int y){
    /*switch(key){
        case 'a':
            gX -= 0.01;
            break;
        case 's':
            gY -= 0.01;
            break;
        case 'w':
            gY += 0.01;
            break;
        case 'd':
            gX += 0.01;
    }
    glutPostRedisplay();*/
    keyStatus[key] = 1;
}

void keyUp(unsigned char key, int x, int y){
    keyStatus[key] = 0;
    glutPostRedisplay();
}

void idle(void){
    if(keyStatus['a']){
        gX -= 0.001;
    }
    if(keyStatus['s']){
        gY -= 0.001;
    }
    if(keyStatus['w']){
        gY += 0.001;
    }
    if(keyStatus['d']){
        gX += 0.001;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    y = TAMANHO_JANELA - y;
    float px = (float)x / TAMANHO_JANELA;
    float py = (float)y / TAMANHO_JANELA;
    
    if(abs(px - gX) <= 0.25 && abs(py - gY) <= 0.25){
        inside = 1;
        dragX = px;
        dragY = py;
    }
    else{
        inside = 0;
    }
    
}

void motion(int x, int y){
    y = TAMANHO_JANELA - y;

    float py = (float)y / TAMANHO_JANELA;
    float px = (float)x / TAMANHO_JANELA;

    if(inside){
        gX += (px - dragX);
        gY += (py - dragY);
        dragX = px;
        dragY = py;
    }
}

void display(void){
    /* Limpar todos os pixels  */
    glClear (GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f (0.6, 0.5, 0.4);
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
        glVertex3f (gX - 0.25, gY - 0.25, 0.0);
        glVertex3f (gX + 0.25, gY - 0.25, 0.0);
        glVertex3f (gX + 0.25, gY + 0.25, 0.0);
        glVertex3f (gX - 0.25, gY + 0.25, 0.0);
    glEnd();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void init (void){
    /* selecionar cor de fundo (preto) */
    glClearColor (255.0, 255.0, 255.0, 0.0);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello world");
    init ();
    glutDisplayFunc(display);

    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
