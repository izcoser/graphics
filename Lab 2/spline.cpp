#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

/* Vetor que marca se usuário clicou num ponto de controle. */
int inside[4] = {0};

/* Variável para determinar a quantidade de pontos da curva. (2 <= n <= 100) */
int n = 30;

/* Variáveis para fazer o arrasto de pontos. */
float dragX;
float dragY;

//Pontos de controle da Spline
GLfloat ctrlpoints[4][3] = {
        { 0.1, 0.1, 0.0}, { 0.3, 0.9, 0.0}, 
        {0.7, 0.1, 0.0}, {0.9, 0.9, 0.0}};

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_MAP1_VERTEX_3);

   //Definicao do polinomio com os pontos de controle
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 1, 0, 1, -1, 0);
}

void display(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   
   /* Desenha a curva aproximada por n+1 pontos. */
   
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= n; i++){
        //Avaliacao do polinomio, retorna um vertice (equivalente a um glVertex3fv) 
        glEvalCoord1f((GLfloat) i/(GLfloat)n);
      }
   glEnd();
   
   /* Desenha os pontos de controle. */
   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   
   glutSwapBuffers();
}

/*void reshape(int w, int h)
{
   //Define a porcao visivel da janela
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //Controla o redimensionamento da janela mantendo o aspect ration do objeto
   if (w <= h)
      glOrtho(-size, size, -size*(GLfloat)h/(GLfloat)w, 
               size*(GLfloat)h/(GLfloat)w, -size, size);
   else
      glOrtho(-size*(GLfloat)w/(GLfloat)h, 
               size*(GLfloat)w/(GLfloat)h, -size, size, -size, size);
   
   //Muda para a matriz de trasformacoes (aulas futuras)
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}*/

void mouse(int button, int state, int x, int y){
   if(state == 0){
      y = TAMANHO_JANELA - y;
      float px = (float)x / TAMANHO_JANELA;
      float py = (float)y / TAMANHO_JANELA;
   
      for(int i = 0; i < 4; i++){
         float control_x = ctrlpoints[i][0];
         float control_y = ctrlpoints[i][1];
         if(abs(px - control_x) <= 0.1 && abs(py - control_y) <= 0.1){
            inside[i] = 1;
            dragX = px;
            dragY = py;
         }
         else{
            inside[i] = 0;
         }
      }
   }
}

void motion(int x, int y){
    y = TAMANHO_JANELA - y;

    float py = (float)y / TAMANHO_JANELA;
    float px = (float)x / TAMANHO_JANELA;
   
   for(int i = 0; i < 4; i++){
      if(inside[i]){
         ctrlpoints[i][0] += (px - dragX);
         ctrlpoints[i][1] += (py - dragY);
         dragX = px;
         dragY = py;
      }
   }
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y){
    if(key == '-' && n > 2){
       n--;
    }
    else if(key == '+' && n < 100){
       n++;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   //glutReshapeFunc(reshape);
   glutKeyboardFunc(keyPress);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutMainLoop();
   return 0;
}
