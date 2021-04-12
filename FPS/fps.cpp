/*
 * This program started from tinkering around with the program below:
 * OGL02Animation.cpp: 3D Shapes with animation
 * from page https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
 */
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "point.h"
#include "fire.h"
#include "objloader.h"
#include "enemy.h"

#include <list>

#define INC_KEYIDLE 0.01
#define INC_KEY 1

list<Fire> fires;
list<Enemy> enemies;

Mesh tree;
Mesh tree2;
Mesh lowtree;
Mesh house;
Mesh bullet;
Mesh girl;

/* Global variables */
char title[] = "Walking around with FPS-like controls";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]

Point pyramid(-1.5f, 0.0f, -6.0f);
Point cube(1.5f, 0.0f, -7.0f);

int keyStatus[256] = {0};

Camera cam(Point(-10.0f, 2.0f, 0.0f), Point(0.0f, 0.0f, -1.0f), Point(0.0f, 1.0f, 0.0f));

GLfloat xMouse = 0;
GLfloat yMouse = 0;

// Horizontal angle of the camera in degrees.
GLfloat angle = 0;

// Vertical angle of the camera in degrees.
GLfloat upangle = 0;

GLfloat yaw = -90.0f; // horizontal angle
GLfloat pitch = 0; // vertical angle
// https://learnopengl.com/Getting-started/Camera

int firstMouse = 1;

int mouseAim = 1;


GLfloat radians(GLfloat degrees){
    return degrees * M_PI / 180;
}

Point getNewDirection(GLfloat yaw, GLfloat pitch){
    return Point(
        cos(radians(yaw)) * cos(radians(pitch)),
        sin(radians(pitch)),
        sin(radians(yaw)) * cos(radians(pitch))
    ).normalize();
}

// Rotates with an angle around Y.
void rotateVector(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat angle){
    GLfloat xx = x;
    GLfloat zz = z;

    x = xx * cos(angle) + zz * sin(angle);
    z = -xx * sin(angle) + zz * cos(angle);
}
 
/* Initialize OpenGL Graphics */
void initGL() {
    glutSetCursor(GLUT_CURSOR_NONE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glEnable(GL_CULL_FACE); //?
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    // Create light components.
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat position1[] = { 0.0f, 0.0f, -10.0f, 1.0f };

    // Assign created components to GL_LIGHT0.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //glColorMaterial(GL_FRONT, GL_DIFFUSE); /*cores*/
    //glEnable(GL_COLOR_MATERIAL);
    //glEnable(GL_DEPTH_TEST);

    tree.loadMesh("Tree.obj");
    tree2.loadMesh("Tree2.obj");
    lowtree.loadMesh("lowtree.obj");
    house.loadMesh("House.obj");
    girl.loadMesh("anime.obj");

    enemies.push_back(Enemy(Point(0, 0, 0), girl));
    enemies.push_back(Enemy(Point(20, 0, 20), girl));
    enemies.push_back(Enemy(Point(-20, 0, -20), girl));
    enemies.push_back(Enemy(Point(15, 0, 35), girl));
    enemies.push_back(Enemy(Point(-15, 0, -35), girl));
    //bullet.loadMesh("bullet.obj");
}

void drawObj(double size)
{   
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 128 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(1,0,0);
    glutSolidCube(size);
    //glutSolidSphere(size, 20, 10);
}

void drawFloor(){
    glBegin(GL_LINES);
        for(int i = -100; i < 100; i += 4){
            glVertex3f(i, 0, 0);
            glVertex3f(i, 0, 100);

            glVertex3f(-100, 0, i);
            glVertex3f(0, 0, i);
        }
    glEnd();
}

void drawAim(int size){
    /*https://stackoverflow.com/questions/5467218/opengl-2d-hud-over-3d */ 
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();        ----Not sure if I need this
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    /*
    */
    glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);

    glClear(GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0);
        glVertex2f(width / 2, height / 2 - size);
        glVertex2f(width / 2, height / 2 + size);

        glVertex2f(width / 2 - size, height / 2);
        glVertex2f(width / 2 + size, height / 2);
    glEnd();
    // Making sure we can render 3d again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();        ----and this?

    glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    
    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();                 // Reset the model-view matrix

    gluLookAt(cam.pos.x, cam.pos.y, cam.pos.z, cam.target.x + cam.pos.x, cam.target.y + cam.pos.y, cam.target.z + cam.pos.z, cam.up.x, cam.up.y, cam.up.z);

    glutSolidSphere(0.5, 20, 10);
    drawFloor();

    glPushMatrix();
    glTranslatef(0, 0, -5);
    //glutSolidSphere(0.5, 20, 10);
    tree.draw();
    glTranslatef(-5, 0, 0);
    tree2.draw();
    glTranslatef(-5, 0, 0);
    glPushMatrix();
    glScalef(1.0f / 5.0f, 1.0f / 5.0f, 1.0f / 5.0f);
    lowtree.draw();
    glPopMatrix();
    glTranslatef(0, 0, 25);
    house.draw();
    glTranslatef(0, 0, 10);
    glScalef(1.0f / 4.0f, 1.0f / 4.0f, 1.0f / 4.0f);
    girl.draw();
    //bullet.draw();
    glPopMatrix();

    for(const auto& fire : fires){
        if(fire.valid){
            glPushMatrix();
            glTranslatef(fire.pos.x, fire.pos.y, fire.pos.z);
            glutSolidSphere(0.1, 10, 20);
            glPopMatrix();
        }
    }

    for(auto& e : enemies){
        e.draw();
    }
    drawAim(6);

    glutSwapBuffers();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
void keyboard(unsigned char key, int x, int y){
    switch(key){        
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1;
            break;
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1;
            break;
        case 'w':
        case 'W':
            keyStatus[(int)('w')] = 1;
            break;
        case 's':
        case 'S':
            keyStatus[(int)('s')] = 1;
            break;
        case 'e':
        case 'E':
            keyStatus[(int)('e')] = 1;
            break;
        case 'q':
        case 'Q':
            keyStatus[(int)('q')] = 1;
            break;
        case 'r':
        case 'R':
            keyStatus[(int)('r')] = 1;
            break;
        case 'f':
        case 'F':
            keyStatus[(int)('f')] = 1;
            break;
        case '1':
            mouseAim = !mouseAim;
            break;
        case ' ':
            fires.push_back(Fire(cam.pos, cam.target));
            break;
        glutPostRedisplay();
    }

    printf("pos: %.2f, %.2f\n", cam.pos.z, cam.pos.x);
}

void keyUp(unsigned char key, int x, int y){
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void passiveMotion(int x, int y){
    
    if(!mouseAim){
        return;
    }

    if (firstMouse){
        xMouse = x;
        yMouse = y;
        firstMouse = 0;
    }
  
    float xoffset = x - xMouse;
    float yoffset = yMouse - y; 
    xMouse = x;
    yMouse = y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
        
    cam.target = getNewDirection(yaw, pitch);

    if(x < 100 || y < 100 || x > glutGet(GLUT_WINDOW_WIDTH) - 100 || y > glutGet(GLUT_WINDOW_HEIGHT) - 100){
        firstMouse = 1;
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    }
}

void idle(void){
    static GLdouble previous_time = glutGet(GLUT_ELAPSED_TIME);
    GLdouble current_time, time_diff;
    current_time = glutGet(GLUT_ELAPSED_TIME);
    time_diff = current_time - previous_time;
    previous_time = current_time;

    GLfloat inc = INC_KEYIDLE;
    
    if(keyStatus[(int)('w')]){
        cam.pos += cam.target * time_diff * inc;
    }
    if(keyStatus[(int)('s')]){
        cam.pos -= cam.target * time_diff * inc;
        
    }
    if(keyStatus[(int)('a')]){
        cam.pos -= (cam.target.cross(cam.up)).normalize() * time_diff * inc;
        
    }
    if(keyStatus[(int)('d')]){
        cam.pos += (cam.target.cross(cam.up)).normalize() * time_diff * inc; 
    }
    cam.pos.y = 2;
    
    if(keyStatus[(int)('q')]){
        yaw -= 20 * inc * time_diff;
        cam.target = getNewDirection(yaw, pitch);
    }
    if(keyStatus[(int)('e')]){
        yaw += 20 * inc * time_diff;
        cam.target = getNewDirection(yaw, pitch);
    }
    if(keyStatus[(int)('r')]){
        pitch += 20 * inc * time_diff;
        cam.target = getNewDirection(yaw, pitch);
    }
    if(keyStatus[(int)('f')]){
        pitch -= 20 * inc * time_diff;
        cam.target = getNewDirection(yaw, pitch);
    }

    for(auto& fire : fires){
        if(fire.valid){
            fire.move(time_diff);
        }
    }
    fires.remove_if([](Fire a){return !a.valid;});

    for(auto& e : enemies){
        e.moveTowardsPlayer(cam.pos, time_diff, enemies);
    }

    glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(1280, 720);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(passiveMotion);
    glutIdleFunc(idle);
    initGL();                       // Our own OpenGL initialization
    //glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}