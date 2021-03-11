#ifndef PLAYER_H
#define	PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"
#include <math.h>

// Dimensions

/*
#define paddleHeight 80
#define paddleWidth 10
#define baseHeight 40
#define baseWidth 100
#define radiusWheel 30
*/

class Player {
    Point p;
    GLfloat radius;
    GLfloat nose_radius;
    GLfloat hand_radius;
    GLfloat arm_width;
    GLfloat arm_height;
    GLfloat theta;          /* Player angle.              */

    GLfloat R;
    GLfloat G;
    GLfloat B;

    GLfloat left_alpha;     /* Angle left forearm - head. */ 
    GLfloat left_beta;      /* Angle left forearm - arm.  */

    GLfloat right_alpha;    /* Angle left forearm - head. */ 
    GLfloat right_beta;     /* Angle left forearm - arm.  */

    int punch_status;       /* Only used for the computer.
                            If 0, not punching. 
                            Else if 1, going to punch.
                            Else if 2, retreating punch.  */ 
                             
private:
    void draw_rect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
    void draw_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void draw_dotted_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void draw_hand(Point p, GLfloat R, GLfloat G, GLfloat B);
    void draw_arm(Point p, GLfloat alpha, GLfloat beta);
    void draw_player(Point p, GLfloat left_alpha, GLfloat left_beta, GLfloat right_alpha, GLfloat right_beta);

public:
    Player(Point _p, GLfloat _radius, GLfloat _R, GLfloat _G, GLfloat _B){
        p = _p;
        radius = _radius;
        nose_radius = _radius / 4;
        hand_radius = _radius / 2;
        arm_width = _radius / 4;
        arm_height = 2 * radius;
        theta = 0;
        left_alpha = right_alpha = left_beta = right_beta = 0;
        left_alpha = 135;
        right_alpha = -135;
        left_beta = -135;
        right_beta = 135;
        R = _R;
        G = _G;
        B = _B;

        /*
        gX = 0; 
        gY = -200; 
        gTheta1 = 0; 
        gTheta2 = 0; 
        gTheta3 = 0; 
        gThetaWheel = 0;*/ 
    };
    void draw(){ 
        draw_player(p, left_alpha, left_beta, right_alpha, right_beta);
    };
    void rotate(GLfloat inc);
    void move_y(GLfloat dy, Player p2);
    void change_forearm_angle(GLfloat dy);
    void change_arm_angle(GLfloat dy);
    void reset_angles(void);
    void begin_punch(void);
    int punching(void);
    void retreat_punch(void);
    void punch(void);

    Point get_left_hand_pos(void);
    Point get_right_hand_pos(void);

    int hit(Player p2);

    GLfloat get_x(){
        return p.x;
    };
    GLfloat get_y(){
        return p.y;
    };
    Point get_pos(){
        return p;
    }

    GLfloat get_radius(){
        return radius;
    }
};

#endif	/* ROBO_H */

