#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "tinyxml2.h"
#include "player.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.01

int player_score = 0;
int computer_score = 0;

static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;

void print_score(GLfloat x, GLfloat y){
    glColor3f(1.0, 1.0, 1.0);
    char *tmpStr;
    sprintf(str, "Player %d x %d Computer", player_score, computer_score);
    glRasterPos2f(x, y);
    tmpStr = str;
    while( *tmpStr ){
    glutBitmapCharacter(font, *tmpStr);
    tmpStr++;
    }
}

GLfloat inc = 1;

int circle_1_x = 175;
int circle_1_y = 175;
int circle_1_r = 30;

int circle_2_x = -175;
int circle_2_y = -175;
int circle_2_r = 15;
Player player = Player(Point(circle_1_x, circle_1_y), circle_1_r, 1, 1, 0);
Player computer = Player(Point(circle_2_x, circle_2_y), circle_2_r, 1, 0, 1);
//Key status
int keyStatus[256] = {0};

// Window dimensions
const GLint Width = 700;
const GLint Height = 700;

// Viewing dimensions
const GLint ViewingWidth = 500;
const GLint ViewingHeight = 500;

void renderScene(void){
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);
    player.draw();
    computer.draw();

    if(player.hit(computer)){
        player_score++;
        player.retreat_punch();
    }

    if(computer.hit(player)){
        computer_score++;
        computer.retreat_punch();
    }

    Point pleft = player.get_left_hand_pos();
    Point pright = player.get_right_hand_pos();
    glColor3f(0, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex3f(pleft.x, pleft.y, 0);
        glVertex3f(pright.x, pright.y, 0);
    glEnd();

    print_score(-ViewingHeight / 2 + 2, - ViewingWidth / 2 + 2);
    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y){
    switch (key){
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1;
            break;
        case 's':
        case 'S':
            keyStatus[(int)('s')] = 1;
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1;
            break;
        case 'w':
        case 'W':
            keyStatus[(int)('w')] = 1;
            break;
        case 'f':
            player.change_forearm_angle(INC_KEY);
            break;
        case 'g':
            player.change_forearm_angle(-INC_KEY);
            break;
        case 'c':
            player.change_arm_angle(INC_KEY);
            break;
        case 'v':
            player.change_arm_angle(-INC_KEY);
            break;
        case 'z':
            player.reset_angles();
            break;
        case 'q':
            player.begin_punch();
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y){
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void init(void){
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
            (ViewingWidth/2),     // X coordinate of right edge            
            -(ViewingHeight/2),     // Y coordinate of bottom edge             
            (ViewingHeight/2),     // Y coordinate of top edge             
            -100,     // Z coordinate of the “near” plane            
            100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
}

void idle(void){
    static GLdouble previous_time = glutGet(GLUT_ELAPSED_TIME);
    GLdouble current_time, time_diff;
    //Pega o tempo que passou do inicio da aplicacao
    current_time = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    time_diff = current_time - previous_time;
    //Atualiza o tempo do ultimo frame ocorrido
    previous_time = current_time;

    double inc = INC_KEYIDLE;
    if(keyStatus[(int)('w')]){
        player.move_y(inc, computer, time_diff);
    }
    if(keyStatus[(int)('s')]){
        player.move_y(-inc, computer, time_diff);
    }
    if(keyStatus[(int)('a')]){
        player.rotate(inc, time_diff);
    }
    if(keyStatus[(int)('d')]){
        player.rotate(-inc, time_diff);
    }

    if(player.punching()){
        player.punch(time_diff);
    }
    glutPostRedisplay();
}



int main(int argc, char* argv[]){
    tinyxml2::XMLDocument doc;
    doc.LoadFile("arena_2.svg");
    tinyxml2::XMLElement* rect = doc.FirstChildElement("svg")->FirstChildElement("rect");
    int rect_x = atoi(rect->Attribute("x"));
    int rect_y = atoi(rect->Attribute("y"));
    int rect_width = atoi(rect->Attribute("width"));
    int rect_height = atoi(rect->Attribute("height"));
    const char* rect_fill = rect->Attribute("fill");

    tinyxml2::XMLElement* circle_1 = doc.FirstChildElement("svg")->FirstChildElement("circle");
    circle_1_x = atoi(circle_1->Attribute("cx"));
    circle_1_y = atoi(circle_1->Attribute("cy"));
    circle_1_r = atoi(circle_1->Attribute("r"));
    const char* circle_1_fill = circle_1->Attribute("fill");

    tinyxml2::XMLElement* circle_2 = doc.FirstChildElement("svg")->LastChildElement("circle");
    int circle_2_x = atoi(circle_2->Attribute("cx"));
    int circle_2_y = atoi(circle_2->Attribute("cy"));
    int circle_2_r = atoi(circle_2->Attribute("r"));
    const char* circle_2_fill = circle_2->Attribute("fill");


    printf("rect x = %d, y = %d, width = %d, height = %d, fill = %s\n", rect_x, rect_y, rect_width, rect_height, rect_fill);
    printf("circle_1_x = %d, circle_1_y = %d, circle_1_r = %d, circle_1_fill = %s\n", circle_1_x, circle_1_y, circle_1_r, circle_1_fill);
    printf("circle_2_x = %d, circle_2_y = %d, circle_2_r = %d, circle_2_fill = %s\n", circle_2_x, circle_2_y, circle_2_r, circle_2_fill);

    
    
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Trabalho 2D");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    
    init();
 
    glutMainLoop();

   return 0;
}