#include <stdio.h>
#include "player.h"
#include <math.h>

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
/*
void Player::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B){
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(thetaWheel, 0, 0, 1);
    DesenhaCirc(radiusWheel, R, G, B);
    glPopMatrix();
}
*/
void Player::draw_arm(Point p, GLfloat alpha, GLfloat beta){
    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    glRotatef(alpha, 0, 0, 1);
    draw_rect(arm_height, arm_width, 0, 1, 0);
    glTranslatef(0, arm_height, 0);
    glRotatef(beta, 0, 0, 1);
    draw_rect(arm_height, arm_width, 0, 1, 0);
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
    draw_dotted_circle(radius * 3, 1, 1, 1);
    /* Draw nose. */
    glPushMatrix();
    glTranslatef(0, radius, 0);
    draw_circle(nose_radius, 0, 0, 1);
    glPopMatrix();
    /* Draw arms and hands. */
    draw_arm(Point(-radius, 0), left_alpha, left_beta);
    draw_arm(Point(radius, 0), right_alpha, right_beta);
    glPopMatrix();
    
}

void Player::move_y(GLfloat dy, Player p2){
    /* Converte theta para radianos e adiciona 90 graus. */
    Point increment = Point(20*dy * cos(theta * 2 * M_PI / 360 + M_PI / 2), 20*dy * sin(theta * 2 * M_PI / 360 + M_PI / 2));
    GLfloat distance = p.sum(increment).distance(p2.get_pos());
    if(distance > radius * 3 + p2.get_radius() && distance > radius + p2.get_radius() * 3){
        p = p.sum(increment);
    }
}

void Player::rotate(GLfloat inc){
    theta += 20*inc;
}

void Player::change_forearm_angle(GLfloat dy){
    left_alpha += dy;
    printf("Increasing forearm angle to %.2f\n", left_alpha);
    /* Notes: start: 135. 30 seems to be the good number for punch.
    difference of 105. */
}
void Player::change_arm_angle(GLfloat dy){
    left_beta += dy;
    printf("Increasing arm angle to %.2f\n", left_beta);
    /* Notes: start: -135. -63 seems to be the good number for punch.
    difference of 72 */
}
void Player::reset_angles(void){
    left_alpha = 135;
    left_beta = -135;
}

void Player::begin_punch(void){
    punch_status = 1;
}

int Player::punching(void){
    return punch_status;
}

void Player::retreat_punch(void){
    punch_status = 2;
}

void Player::punch(void){
    /* punch */
    if(punch_status == 0){
        return;
    }
    else if(punch_status == 1){
        left_alpha -= 1.45;
        left_beta += 1;
        if(left_alpha <= 30 || left_beta >= -63){
            punch_status = 2;
        }
    }
    else if(punch_status == 2){
        left_alpha += 1.45;
        left_beta -= 1;
        if(left_alpha >= 135 || left_beta <= -135){
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

    return left_hand.distance(p2_pos) <= hand_radius + p2.get_radius() || right_hand.distance(p2_pos) <= hand_radius + p2.get_radius(); 
}

/*
void Robo::RodaBraco1(GLfloat inc){
    this->gTheta1 += inc;

}

void Robo::RodaBraco2(GLfloat inc){
    this->gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc){
    this->gTheta3 += inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    gX += 20*dx;
    gThetaWheel -= 20*dx / 2 * M_PI;
}

void RotatePoint(GLfloat &px, GLfloat &py, GLfloat angle, GLfloat ox, GLfloat oy){
    printf("Rotating %.2f, %.2f by %.2f, %.2f at angle %.2f\n", px, py, ox, oy, angle);
    GLfloat s = sin(angle);
    GLfloat c = cos(angle);

    px -= ox;
    py -= oy;

    GLfloat xnew = px * c - py * s;
    GLfloat ynew = px * s + py * c;

    px = xnew + ox;
    py = ynew + oy;
    printf("Result %.2f, %.2f\n", px, py);
}

Tiro* Robo::Atira(){
    GLfloat aux_x = gX;
    GLfloat aux_y = gY + baseHeight;

    GLfloat tiro_x = gX;
    GLfloat tiro_y = gY + baseHeight + paddleHeight;

    printf("First rotation:\n");
    RotatePoint(tiro_x, tiro_y, gTheta1 * 2 * M_PI / 360, aux_x, aux_y);
    aux_x = tiro_x;
    aux_y = tiro_y;

    tiro_y += paddleHeight;
    
    printf("Second rotation:\n");
    RotatePoint(tiro_x, tiro_y, (gTheta1 + gTheta2)* 2 * M_PI / 360, aux_x, aux_y);
    aux_x = tiro_x;
    aux_y = tiro_y;

    tiro_y += paddleHeight;

    printf("Third rotation:\n");
    RotatePoint(tiro_x, tiro_y, (gTheta1 + gTheta2 + gTheta3) * 2 * M_PI / 360, aux_x, aux_y);

    GLfloat gDirection = atan2((double)tiro_y - (double)aux_y, (double)tiro_x - (double)aux_x);

    Tiro* t = new Tiro(tiro_x, tiro_y, gDirection);
    return t;
}
*/