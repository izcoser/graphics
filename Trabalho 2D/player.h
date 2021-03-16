#ifndef PLAYER_H
#define	PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "point.h"
#include <math.h>

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

    GLfloat R_border;
    GLfloat G_border;
    GLfloat B_border;

    GLfloat R_arm;
    GLfloat G_arm;
    GLfloat B_arm;

    GLfloat left_alpha;     /* Angle left forearm - head. */ 
    GLfloat left_beta;      /* Angle left forearm - arm.  */

    GLfloat right_alpha;    /* Angle left forearm - head. */ 
    GLfloat right_beta;     /* Angle left forearm - arm.  */

    int punch_status;       /* Only used for the computer.
                            If 0, not punching. 
                            Else if 1, going to punch (left).
                            Else if 2, retreating punch (left).
                            Else if 3, going to punch (right).
                            Else if 4, retreating punch (right).
                            */

    int score;
    int won;
    int movement;           /* Only used for the computer.
                            If 0, moving away. If 1, towards
                            player.
                            */

    int can_hit;            /* A hit will only count if
                            can_hit = 1. To stop hits from
                            counting multiple times. */
                             
private:
    void draw_rect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
    void draw_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void draw_empty_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void draw_dotted_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void draw_hand(Point p, GLfloat R, GLfloat G, GLfloat B);
    void draw_arm(Point p, GLfloat alpha, GLfloat beta);
    void draw_player(Point p, GLfloat left_alpha, GLfloat left_beta, GLfloat right_alpha, GLfloat right_beta);

public:
    void init(Point _p, GLfloat _radius, GLfloat _R, GLfloat _G, GLfloat _B){
        p = _p;
        radius = _radius;
        nose_radius = _radius / 4;
        hand_radius = _radius / 2;
        arm_width = _radius / 4;
        arm_height = 2 * radius;
        theta = 0;
        left_alpha = 135;
        right_alpha = -135;
        left_beta = -135;
        right_beta = 135;
        R = _R;
        G = _G;
        B = _B;
        score = 0;
        won = 0;
        movement = 0;
        can_hit = 1;

        if(G == 1.0f){
            R_border = 33.0/255;
            G_border = 110.0/255;
            B_border = 33.0/255;

            R_arm = 155.0/255;
            G_arm = 187.0/255;
            B_arm = 89.0/255;
        }
        else{
            R_border = 110.0/255;
            G_border = 33.0/255;
            B_border = 33.0/255;
            
            R_arm = 95.0/255;
            G_arm = 140.0/255;
            B_arm = 6.0/255;
        }
    }

    void draw(){ 
        draw_player(p, left_alpha, left_beta, right_alpha, right_beta);
    };
    void rotate(GLfloat inc, GLdouble time_diff);
    void move(GLfloat dy, Player p2, GLdouble time_diff, GLint view_width, GLint view_height);
    void change_forearm_angle(GLfloat dy);
    void change_arm_angle(GLfloat dy);
    void reset_angles(void);
    
    void begin_left_punch(void);
    void begin_right_punch(void);
    
    int punching(void);
    void set_punch_status(int status);
    
    void retreat_left_punch(void);
    void retreat_right_punch(void);
    void retreat_punch(void);

    void punch(GLdouble time_diff);

    Point get_left_hand_pos(void);
    Point get_right_hand_pos(void);

    int hit(Player p2);
    void disable_hit(void);
    void enable_hit(void);
    int hit_enabled(void);

    void update_left_arm_angles(GLfloat left_alpha, GLfloat left_beta);
    void update_right_arm_angles(GLfloat right_alpha, GLfloat right_beta);

    int get_score(void);
    void increase_score(void);

    void set_win(void);
    int winner(void);
    
    void look_at(Player p2);

    int moving_towards_player(void);
    void change_movement(void);

    int in_punching_distance(Player p2);

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

#endif

