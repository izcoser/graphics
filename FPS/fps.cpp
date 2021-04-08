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

#define INC_KEYIDLE 0.01
#define INC_KEY 1
 
/* Global variables */
char title[] = "Walking around with FPS-like controls";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]

Point pyramid(-1.5f, 0.0f, -6.0f);
Point cube(1.5f, 0.0f, -7.0f);

int keyStatus[256] = {0};

Camera cam(Point(0.0f, 0.0f, 0.0f), Point(0.0f, 0.0f, -100.0f), Point(0.0f, 1.0f, 0.0f));

GLfloat xMouse = 0;

// Angle between (zAt, xAt) and (1, 0), radians.
GLfloat angle = M_PI;

// Rotates with an angle around Y.
void rotateVector(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat angle){
    GLfloat xx = x;
    GLfloat zz = z;

    x = xx * cos(angle) + zz * sin(angle);
    z = -xx * sin(angle) + zz * cos(angle);
}
 
/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    /*GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);*/

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

    glColorMaterial(GL_FRONT, GL_DIFFUSE); /*cores*/
    glEnable(GL_COLOR_MATERIAL);
    //glEnable(GL_DEPTH_TEST);
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   gluLookAt(cam.p.x, cam.p.y, cam.p.z, cam.lookat.x, cam.lookat.y, cam.lookat.z, cam.up.x, cam.up.y, cam.up.z);
   //glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen
   glPushMatrix();
   glTranslatef(cube.x, cube.y, cube.z);
   glRotatef(angleCube, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW]

   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube
   glPopMatrix();
 
   // Render a pyramid consists of 4 triangles
   //glLoadIdentity();                  // Reset the model-view matrix
   glPushMatrix();
   glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen
   glRotatef(anglePyramid, 1.0f, 1.0f, 0.0f);  // Rotate about the (1,1,0)-axis [NEW]
    

   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
 
      // Right
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glColor3f(1.0f,0.0f,0.0f);       // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f,0.0f,1.0f);       // Blue
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glColor3f(0.0f,1.0f,0.0f);       // Green
      glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
   glPopMatrix();
 
    //glutSolidSphere (1.0, 20, 16);
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
 
   // Update the rotational angle after each refresh [NEW]
   //anglePyramid += 0.2f;
   //angleCube -= 0.15f;
}
 
/* Called back when timer expired [NEW]
void timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()    
    glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}*/

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
    int width = glutGet(GLUT_WINDOW_WIDTH);
    GLfloat rotationAngle;
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
        case 'q':
            rotationAngle = (1 / (GLfloat)width) * 2 * M_PI;
            rotateVector(cam.lookat.x, cam.lookat.y, cam.lookat.z, rotationAngle);
            printf("xAt: %.2f, zAt: %.2f\n", cam.lookat.x, cam.lookat.z);
            angle += rotationAngle;
            break;
        case 'e':
            rotationAngle = (-1 / (GLfloat)width) * 2 * M_PI;
            rotateVector(cam.lookat.x, cam.lookat.y, cam.lookat.z, rotationAngle);
            printf("xAt: %.2f, zAt: %.2f\n", cam.lookat.x, cam.lookat.z);
            angle += rotationAngle;
            break;

        glutPostRedisplay();
    }

    printf("pos: %.2f, %.2f\n", cam.p.z, cam.p.x);
}

void keyUp(unsigned char key, int x, int y){
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void passiveMotion(int x, int y){

    GLfloat diff = xMouse - x;
    xMouse = x;
    if(abs(diff) > 30){ // avoid sudden camera rotations when mouse enters screen
        return;
    }
    else{
        int width = glutGet(GLUT_WINDOW_WIDTH);
        GLfloat rotationAngle = (diff / (GLfloat)width) * 2 * M_PI;
        rotateVector(cam.lookat.x, cam.lookat.y, cam.lookat.z, rotationAngle);
        printf("xAt: %.2f, zAt: %.2f\n", cam.lookat.x, cam.lookat.z);
        angle += rotationAngle;
        //angle = atan2(zAt, xAt) - M_PI / 2;
        printf("Angle: %.2f, %.2f\n", angle, angle * 180 / M_PI);

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
        cam.p.z += inc * cos(angle) * time_diff;
        cam.p.x += inc * sin(angle) * time_diff;
    }
    if(keyStatus[(int)('s')]){
        cam.p.z -= inc * cos(angle) * time_diff;
        cam.p.x -= inc * sin(angle) * time_diff;
    }
    if(keyStatus[(int)('a')]){
        cam.p.x += inc * cos(angle) * time_diff;
        cam.p.z -= inc * sin(angle) * time_diff;
    }
    if(keyStatus[(int)('d')]){
        cam.p.x -= inc * cos(angle) * time_diff;
        cam.p.z += inc * sin(angle) * time_diff;
    }
    glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
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