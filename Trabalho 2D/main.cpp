#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>

#include "string.h"
#include "math.h"
#include "tinyxml2.h"
#include "player.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.01

#define min(a,b) ((a) < (b) ? (a) : (b))

Player player;
Player computer;

int click_x;
int computer_enabled = 1;


static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;

void print_score(GLfloat x, GLfloat y){
    glColor3f(1.0, 1.0, 1.0);
    char *tmpStr;
    sprintf(str, "Jogador %d x %d Computador", player.get_score(), computer.get_score());
    glRasterPos2f(x, y);
    tmpStr = str;
    while( *tmpStr ){
    glutBitmapCharacter(font, *tmpStr);
    tmpStr++;
    }
}

void print_win_message(GLfloat x, GLfloat y){
    glColor3f(1.0, 1.0, 1.0);
    char *tmpStr;
    if(player.winner()){
        sprintf(str, "Voce venceu por %d x %d!", player.get_score(), computer.get_score());    
    }
    else{
        sprintf(str, "Voce perdeu por %d x %d!", player.get_score(), computer.get_score());
    }
    glRasterPos2f(x, y);
    tmpStr = str;
    while( *tmpStr ){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmpStr);
    tmpStr++;
    }
}

GLfloat inc = 1;

//Key status
int keyStatus[256] = {0};

// Window dimensions
GLint rect_width;
GLint rect_height;

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT);
    player.draw();
    computer.draw();
    print_score(2, 2);

    if(player.winner() || computer.winner()){
        print_win_message(rect_width / 2 - 80, rect_height / 2);
    }
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
        case '1':
            computer.change_movement();
            break;
        case '2':
            computer_enabled = !computer_enabled;
            break;
        case 'q':
            exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y){
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if(state == 0){
        click_x = x;
        printf("Clicked at x = %d\n", x);
    }
    else if(state == 1){
        player.set_punch_status(0);
        player.reset_angles(); /* should modify to return naturally
                                maybe changing punch status.*/
    }
}

void motion(int x, int y){
    printf("Motion at x = %d\n", x);
    GLfloat punch_percentage = min(1, (GLfloat)(abs(x - click_x)) / (rect_width / 2));
    printf("Punch percentage: %.2f\n", punch_percentage);
    if(x > click_x){
        /* Right punch*/
        player.set_punch_status(3);
        GLfloat right_alpha = -135 + 105 * punch_percentage;
        GLfloat right_beta = 135 - 72 * punch_percentage;
        player.update_right_arm_angles(right_alpha, right_beta);
    }
    else if(x < click_x){
        /* Left punch */
        player.set_punch_status(1);
        GLfloat left_alpha = 135 - 105 * punch_percentage;
        GLfloat left_beta = -135 + 72 * punch_percentage;
        player.update_left_arm_angles(left_alpha, left_beta);
    }
    
}

void init(void){
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0, 0, 1, 1.0f);
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glOrtho(0, rect_width, 0, rect_height, -100, 100); // x_left, x_right, y_bottom, y_top, z_near, z_far
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();
}

int game_ended(void){
    return player.get_score() >= 10 || computer.get_score() >= 10;
}

void idle(void){
    if(game_ended()){
        return;
    }


    static GLdouble previous_time = glutGet(GLUT_ELAPSED_TIME);
    static GLdouble computer_timer = 0;
    GLdouble current_time, time_diff;
    //Pega o tempo que passou do inicio da aplicacao
    current_time = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    time_diff = current_time - previous_time;
    computer_timer += time_diff;
    //Atualiza o tempo do ultimo frame ocorrido
    previous_time = current_time;

    computer.look_at(player);

    double inc = INC_KEYIDLE;
    if(keyStatus[(int)('w')]){
        player.move(inc, computer, time_diff, rect_width, rect_height);
    }
    if(keyStatus[(int)('s')]){
        player.move(-inc, computer, time_diff, rect_width, rect_height);
    }
    if(keyStatus[(int)('a')]){
        player.rotate(inc, time_diff);
    }
    if(keyStatus[(int)('d')]){
        player.rotate(-inc, time_diff);
    }

    if(computer_enabled){
        if(computer.moving_towards_player()){
        computer.move(inc, player, time_diff, rect_width, rect_height);
            if(!computer.punching() && computer.get_pos().distance(player.get_pos()) <= 3 * computer.get_radius() + 1.5 * player.get_radius()){
                if(rand() % 2){
                    computer.begin_left_punch();
                }
                else{
                    computer.begin_right_punch();
                }
            }
        }
        else{
            computer.move(-inc, player, time_diff, rect_width, rect_height);
        }
    }
    
    if(computer.punching()){
        computer.punch(time_diff);
    }

    if(player.hit(computer)){
        if(player.hit_enabled()){
            player.increase_score();
            player.disable_hit();
            player.retreat_punch();
        }
    }
    else{
        player.enable_hit();
    }

    if(computer.hit(player)){
        if(computer.hit_enabled()){
            computer.increase_score();
            computer.disable_hit();
            computer.retreat_punch();
        }
    }
    else{
        computer.enable_hit();
    }

    if(player.get_score() >= 10){
        player.set_win();
    } 
    else if(computer.get_score() >= 10){
        computer.set_win();
    }

    if(computer_timer > 5000){
        computer_timer = 0;
        computer.change_movement();
    }

    glutPostRedisplay();
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    tinyxml2::XMLDocument doc;
    doc.LoadFile("arena_2.svg");
    tinyxml2::XMLElement* rect = doc.FirstChildElement("svg")->FirstChildElement("rect");
    int rect_x = atoi(rect->Attribute("x"));
    int rect_y = atoi(rect->Attribute("y"));

    rect_width = (GLfloat)atof(rect->Attribute("width"));
    rect_height = (GLfloat)atof(rect->Attribute("height"));
    const char* rect_fill = rect->Attribute("fill");

    tinyxml2::XMLElement* circle_1 = doc.FirstChildElement("svg")->FirstChildElement("circle");
    int circle_1_x = atoi(circle_1->Attribute("cx")) - rect_x;
    int circle_1_y = atoi(circle_1->Attribute("cy")) - rect_y;
    int circle_1_r = atoi(circle_1->Attribute("r"));
    const char* circle_1_fill = circle_1->Attribute("fill");

    tinyxml2::XMLElement* circle_2 = doc.FirstChildElement("svg")->LastChildElement("circle");
    int circle_2_x = atoi(circle_2->Attribute("cx")) - rect_x;
    int circle_2_y = atoi(circle_2->Attribute("cy")) - rect_y;
    int circle_2_r = atoi(circle_2->Attribute("r"));
    const char* circle_2_fill = circle_2->Attribute("fill");


    printf("rect x = %d, y = %d, width = %d, height = %d, fill = %s\n", rect_x, rect_y, rect_width, rect_height, rect_fill);
    printf("circle_1_x = %d, circle_1_y = %d, circle_1_r = %d, circle_1_fill = %s\n", circle_1_x, circle_1_y, circle_1_r, circle_1_fill);
    printf("circle_2_x = %d, circle_2_y = %d, circle_2_r = %d, circle_2_fill = %s\n", circle_2_x, circle_2_y, circle_2_r, circle_2_fill);

    if(strcmp(circle_1_fill, "green") == 0){
        player.init(Point(circle_1_x, circle_1_y), circle_1_r, 0, 1, 0);
        computer.init(Point(circle_2_x, circle_2_y), circle_2_r, 1, 0, 0);    
    }
    else{
        player.init(Point(circle_2_x, circle_2_y), circle_2_r, 0, 1, 0);
        computer.init(Point(circle_1_x, circle_1_y), circle_1_r, 1, 0, 0);
    }

    player.look_at(computer);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    // Create the window.
    glutInitWindowSize(rect_width, rect_height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Trabalho 2D");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
 
    glutMainLoop();

   return 0;
}