#include <stdio.h>
#include "player.h"
#include <math.h>

#define PUNCH_CONSTANT 0.6

void Player::draw_rect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
        glVertex3f( - (GLfloat)width / 2, 0, 0);
        glVertex3f(   (GLfloat)width / 2, 0, 0);
        glVertex3f(   (GLfloat)width / 2, (GLfloat)height, 0);
        glVertex3f( - (GLfloat)width / 2, (GLfloat)height, 0);
    glEnd();
}

void Player::draw_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for(int i = 0; i <= 20; i++){
            glVertex3f (radius * cos(i * 2 * M_PI / 20), radius * sin(i * 2 * M_PI / 20), 0);
        }
    glEnd();
}

void Player::draw_dotted_circle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B){
    glPointSize(3);
    glColor3f(R, G, B);
    glBegin(GL_POINTS);
        for (int i = 0; i <= 20; i++){
            glVertex3f (radius * cos(i * 2 * M_PI / 20), radius * sin(i * 2 * M_PI / 20), 0);
        }
    glEnd();    
}

void Player::draw_arm(Point p, GLfloat alpha, GLfloat beta){
    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    glRotatef(alpha, 0, 0, 1);
    draw_rect(arm_height, arm_width, 155.0/255, 187.0/255, 89.0/255);
    glTranslatef(0, arm_height, 0);
    glRotatef(beta, 0, 0, 1);
    draw_rect(arm_height, arm_width, 155.0/255, 187.0/255, 89.0/255);
    glTranslatef(0, arm_height, 0);
    /* Hand */
    draw_circle(hand_radius, 1, 0, 0);
    glPopMatrix();
}

void Player::draw_player(Point p, GLfloat left_alpha, GLfloat left_beta, GLfloat right_alpha, GLfloat right_beta){
    glPushMatrix();
    //printf("Drawing player at %.2f, %.2f\n", p.x, p.y);
    glTranslatef(p.x, p.y, 0);
    glRotatef(theta, 0, 0, 1);
    /* Draw head. */
    draw_circle(radius, R, G, B);
    /* Draw collision radius. */
    //draw_dotted_circle(radius * 3, 1, 1, 1);
    /* Draw nose. */
    glPushMatrix();
    glTranslatef(0, radius, 0);
    draw_circle(nose_radius, R, G, B);
    glPopMatrix();
    /* Draw arms and hands. */
    draw_arm(Point(-radius, 0), left_alpha, left_beta);
    draw_arm(Point(radius, 0), right_alpha, right_beta);
    glPopMatrix();
    
}

int inside_arena(Point p, GLfloat radius, GLint view_width, GLint view_height){
    return p.x - radius > 0 && p.x + radius < view_width \
    && p.y - radius > 0 && p.y + radius < view_height; 
}

void Player::move(GLfloat dy, Player p2, GLdouble time_diff, GLint view_width, GLint view_height){
    /* Converte theta para radianos e adiciona 90 graus. */
    Point increment = Point(20*dy * cos(theta * 2 * M_PI / 360 + M_PI / 2) * time_diff, 20*dy * sin(theta * 2 * M_PI / 360 + M_PI / 2) * time_diff);
    Point new_point = p.sum(increment);
    GLfloat distance = new_point.distance(p2.get_pos());
    if(!punching() && distance > radius * 3 + p2.get_radius() && distance > radius + p2.get_radius() * 3 \
    && inside_arena(new_point, radius, view_width, view_height)){
        p = new_point;
    }
}

void Player::rotate(GLfloat inc, GLdouble time_diff){
    theta += 20* inc * time_diff;
}

void Player::change_forearm_angle(GLfloat dy){
    left_alpha += dy;
    /* Notes: start: 135. 30 seems to be the good number for punch.
    difference of 105. */
}
void Player::change_arm_angle(GLfloat dy){
    left_beta += dy;
    /* Notes: start: -135. -63 seems to be the good number for punch.
    difference of 72 */
}
void Player::reset_angles(void){
    left_alpha = 135;
    left_beta = -135;
    right_alpha = -135;
    right_beta = 135;
}

void Player::begin_left_punch(void){
    punch_status = 1;
}

void Player::begin_right_punch(void){
    punch_status = 3;
}

int Player::punching(void){
    return punch_status;
}

void Player::set_punch_status(int status){
    punch_status = status;
}

void Player::retreat_left_punch(void){
    punch_status = 2;
}

void Player::retreat_right_punch(void){
    punch_status = 4;
}

void Player::retreat_punch(void){
    if(left_alpha < 135){
        /* It's a left punch. */
        punch_status = 2;
    }
    else{
        /* It's a right punch. */
        punch_status = 4;
    }
}

void Player::punch(GLdouble time_diff){
    if(punch_status == 0){
        return;
    }
    else if(punch_status == 1){
        left_alpha -= 1.45 * time_diff * PUNCH_CONSTANT;
        left_beta += 1 * time_diff * PUNCH_CONSTANT;
        if(left_alpha <= 30 || left_beta >= -63){
            punch_status = 2;
        }
    }
    else if(punch_status == 2){
        left_alpha += 1.45 * time_diff * PUNCH_CONSTANT;
        left_beta -= 1 * time_diff * PUNCH_CONSTANT;
        if(left_alpha >= 135 || left_beta <= -135){
            punch_status = 0;
            reset_angles();
        }
    }
    else if(punch_status == 3){
        right_alpha += 1.45 * time_diff * PUNCH_CONSTANT;
        right_beta -= 1 * time_diff * PUNCH_CONSTANT;
        if(right_alpha >= -30 || right_beta <= 63){
            punch_status = 4;
        }
    }
    else if(punch_status == 4){
        right_alpha -= 1.45 * time_diff * PUNCH_CONSTANT;
        right_beta += 1 * time_diff * PUNCH_CONSTANT;
        if(right_alpha <= -135 || right_beta >= 135){
            punch_status = 0;
            reset_angles();
        }
    }
}

Point Player::get_left_hand_pos(void){
    Point hand(0, 0);
    hand = hand.sum(Point(0, arm_height));
    hand = hand.rotate(left_beta);
    hand = hand.sum(Point(0, arm_height));
    hand = hand.rotate(left_alpha);
    hand = hand.sum(Point(-radius, 0));
    hand = hand.rotate(theta);
    hand = hand.sum(p);

    return hand;
}

Point Player::get_right_hand_pos(void){
    Point hand(0, 0);
    hand = hand.sum(Point(0, arm_height));
    hand = hand.rotate(right_beta);
    hand = hand.sum(Point(0, arm_height));
    hand = hand.rotate(right_alpha);
    hand = hand.sum(Point(radius, 0));
    hand = hand.rotate(theta);
    hand = hand.sum(p);

    return hand;
}

int Player::hit(Player p2){
    Point left_hand = get_left_hand_pos();
    Point right_hand = get_right_hand_pos();
    Point p2_pos = p2.get_pos();

    return (left_hand.distance(p2_pos) <= hand_radius + p2.get_radius() || right_hand.distance(p2_pos) <= hand_radius + p2.get_radius()); 
}

void Player::disable_hit(void){
    can_hit = 0;
}

void Player::enable_hit(void){
    can_hit = 1;
}

int Player::hit_enabled(void){
    return can_hit;
}

void Player::update_left_arm_angles(GLfloat left_alpha, GLfloat left_beta){
    /* Left alpha starts at 135, goes down to 30. */
    /* Left beta starts at -135, goes up to -63. */
    this->left_alpha = left_alpha;
    this->left_beta = left_beta;
}
void Player::update_right_arm_angles(GLfloat right_alpha, GLfloat right_beta){
    /* Right alpha starts at -135, goes up to -30. */
    /* Right beta starts at 135, goes down to 63. */
    this->right_alpha = right_alpha;
    this->right_beta = right_beta;
}

int Player::get_score(void){
    return score;
}

void Player::increase_score(void){
    score++;
}

void Player::set_win(void){
    won = 1;
}

int Player::winner(void){
    return won;
}

void Player::look_at(Player p2){
    Point p2_pos = p2.get_pos();
    theta = atan2(p2_pos.y - p.y, p2_pos.x - p.x) * 180 / M_PI - 90;
}

int Player::moving_towards_player(void){
    return movement;
}

void Player::change_movement(void){
    movement = !movement;
}