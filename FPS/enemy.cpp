#include "enemy.h"
#include "point.h"
#include "camera.h"
#include "math.h"
#include <list>

Enemy::Enemy(Point pos, Mesh mesh){
    this->pos = pos;
    this->mesh = mesh;
}

void Enemy::draw(void){
    glPushMatrix();
    glTranslatef(pos.x, pos.y - 2, pos.z);
    glRotatef(angle * 180 / M_PI, 0, 1, 0);
    glScalef(0.8, 0.8, 0.8);
    mesh.draw();
    glPopMatrix();
}

void Enemy::moveTowardsPlayer(Point pos, GLdouble timeDiff, list<Enemy> enemies){
    this->lookAt(pos);
    this->target = (pos - this->pos).normalize();
    Point newPos = this->target * timeDiff * 0.008;
    /*for(auto& e : enemies){
        if(e.pos.isClose(newPos, 1)){
            return;
        }
    }*/
    this->pos += newPos;
    //this->pos += this->target * timeDiff * 0.008;

}

void Enemy::lookAt(Point pos){
    angle = atan2(this->pos.x - pos.x, this->pos.z - pos.z) + M_PI;
}