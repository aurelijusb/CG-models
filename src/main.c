/**
 * Computer graphics, task 3. 
 * 3D objects
 * 
 * @author Aurelijus Banelis
 */

//Tada parašome OpenGL  programą, kuri:
//    rhombic triacontahedron
//    vaizduoja briaunainio 3d modelį, kurio gretimos sienos turi skirtingas spalvas
//    sukurti vartotojo sąsają, kuri leidžia briaunainį apžiūrėti iš visų pusių (pvz. view_torus.cpp)
//    nustatyti tris posūkius (ašis+kampas), kurie generuoja visus posūkius pervedančius briaunainį į save
//    žaidimas: pasukti briaunainį į specialią padėtį animuojant pasirenkamus posūkius (vartotojo sąsaja turi būti patogi)


#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//
//#define KEY_ESCAPE 27
//#define KEY_ENTER 13
//
#define FALSE 0
#define TRUE 1

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <math.h>       /* for cos(), sin(), and sqrt() */
//#include <GL/glut.h>
#include "trackball.h"

#define PI 3.141592653589793
#define RAD(ange) (ange * PI / 180.0)
#define SIN_21 0.3583679495453
#define COS_21 0.9335804264972
#define SIN_58 0.8480480961564
#define COS_58 0.5299192642332

GLfloat axis[3][3] = {{0, 1, 0},
                      {SIN_21, COS_21, 0},
                      {SIN_58, COS_58, 0},
                     };
GLfloat axis2[3][3] = {{0, 1, 0},
                      {SIN_21, COS_21, 0},
                      {SIN_58, COS_58, 0},
                     };
int angles[3] = {180, 120, 72};

GLfloat angle = -150;   /* in degrees */
GLboolean doubleBuffer = GL_TRUE, iconic = GL_FALSE, keepAspect = GL_FALSE;
int spinning = 0, moving = 0;
int beginx, beginy;
int W = 600, H = 600;
float empty[4];
float curquat[4];
float lastquat[4];
float gamequat[4];
GLdouble bodyWidth = 3.0;
int newModel = 1;
int scaling;
float scalefactor = 1.0;
int r[3][2] = {{0, 0}, {0, 0}, {0, 0}};
short showAxis = 0;
short game = 0;

#define NV 32
#define PHI1 1.6180339887499
#define PHI2 (PHI1*PHI1)
#define PHI3 (PHI1*PHI1*PHI1)
float vertexes[NV][3] = {{PHI2, PHI2, PHI2}, 
                        {PHI2, PHI2, -PHI2}, 
                        {PHI2, -PHI2, PHI2}, 
                        {PHI2, -PHI2, -PHI2}, 
                        {-PHI2, PHI2, PHI2}, 
                        {-PHI2, PHI2, -PHI2}, 
                        {-PHI2, -PHI2, PHI2}, 
                        {-PHI2, -PHI2, -PHI2}, 
                        {PHI1, PHI3, -0.00}, 
                        {-PHI1, PHI3, -0.00}, 
                        {PHI1, -PHI3, -0.00}, 
                        {-PHI1, -PHI3, 0.000}, 
                        {PHI3, -0.00, PHI1}, 
                        {PHI3, -0.00, -PHI1}, 
                        {-PHI3, -0.00, PHI1}, 
                        {-PHI3, 0.000, -PHI1}, 
                        {-0.00, PHI1, PHI3}, 
                        {-0.00, -PHI1, PHI3}, 
                        {-0.00, PHI1, -PHI3}, 
                        {0.000, -PHI1, -PHI3}, 
                        {-0.00, PHI3, PHI2}, 
                        {PHI3, PHI2, -0.00}, 
                        {PHI2, -0.00, PHI3}, 
                        {-0.00, PHI3, -PHI2}, 
                        {PHI3, -PHI2, -0.00}, 
                        {-PHI2, -0.00, PHI3}, 
                        {-PHI3, PHI2, -0.00}, 
                        {-0.00, -PHI3, PHI2}, 
                        {PHI2, -0.00, -PHI3}, 
                        {-PHI2, -0.00, -PHI3}, 
                        {-PHI3, -PHI2, -0.00}, 
                        {-0.00, -PHI3, -PHI2}};

/*
 * Models
 */

void drawLineY() {
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
    glEnd();
}

void drawAxis() {
    // X (by y)
    glColor3f(1, 0, 0);
    drawLineY();
    
    //Y (by x)
    glColor3f(0, 1, 0);
    glPushMatrix();
        glRotatef(-90, 0, 0, 1);
        drawLineY();
    glPopMatrix();
    
    //Z (by XY)
    glColor3f(0.2, 0.2, 1);
    glPushMatrix();
        glRotatef(-45, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        drawLineY();
    glPopMatrix();
}

void drawRotationAxis() {
    float colors[3][3] = {{0.5, 0, 0},
                          {0, 0.5, 0},
                          {0, 0, 0.5}
                         };
    const int n = 3;
    int i;
    int radius = 10;
    for (i = 0; i < n; i++) {
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0, 0, 0);
            glVertex3f(axis[i][0] * radius, axis[i][1] * radius, axis[i][2] * radius);
        glEnd();
    }
}

void drawObject() {
    int i, j, k, l, m;
    
    float phi = (1+sqrt(5))/2;
    float phi1 = phi;
    float phi2 = phi*phi;
    float phi3 = phi*phi*phi;  
    
#define NF 30
    
    short faces[NF][4] = {{0, 20, 16, 22}, 
                          {0, 22, 12, 21}, 
                          {0, 21, 8, 20}, 
                          {1, 21, 13, 28}, 
                          {1, 28, 18, 23}, 
                          {1, 23, 8, 21}, 
                          {2, 22, 17, 27}, 
                          {2, 27, 10, 24}, 
                          {2, 24, 12, 22}, 
                          {3, 24, 10, 31}, 
                          {3, 31, 19, 28}, 
                          {3, 28, 13, 24}, 
                          {4, 20, 9, 26}, 
                          {4, 26, 14, 25}, 
                          {4, 25, 16, 20}, 
                          {5, 23, 18, 29}, 
                          {5, 29, 15, 26}, 
                          {5, 26, 9, 23}, 
                          {6, 25, 14, 30}, 
                          {6, 30, 11, 27}, 
                          {6, 27, 17, 25}, 
                          {7, 29, 19, 31}, 
                          {7, 31, 11, 30}, 
                          {7, 30, 15, 29}, 
                          {8, 23, 9, 20}, 
                          {10, 27, 11, 31}, 
                          {12, 24, 13, 21}, 
                          {14, 26, 15, 30}, 
                          {16, 25, 17, 22}, 
                          {18, 28, 19, 29}};

#define NN 12
    short neighbors[NF][NN] = {0};
    for (i=0; i<NF; i++) {
        for (j=0; j<NN; j++) {
            neighbors[i][j] = -1;
        }
    }

    
    //Source face
    for(i=0; i < NF; i++) {
        //Destination face
        for(j=0; j < NF; j++) {
            short neighbor = FALSE;
            //Source vertex
            for (k=0; k < 4 && !neighbor && i != j; k++) {
                //Destination vertex
                for (l=0; l < 4 && !neighbor; l++) {
                    // Cecking if neighbor
                    if (faces[i][k] == faces[j][l]) {
                        neighbor = TRUE;
                        //Adding to neighbors list
                        short exists = FALSE;
                        for (m=0; m < NN && !exists; m++) {
                            if (neighbors[i][m] == j) {
                                exists = TRUE;
                            } else if (neighbors[i][m] == -1) {
                                neighbors[i][m] = j;
                                exists = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
    
#define GL_VERTER_ARRAY(array) glVertex3f(array[0], array[1], array[2]) 
#define GL_COLORS_ARRAY(array) glColor3f(array[0], array[1], array[2]) 
    
    //Vertexes
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
        for (i = 0; i < NV; i++) {
            GL_VERTER_ARRAY(vertexes[i]);
        }
    glEnd();
    
    glColor3f(0, 1, 1);
    for (i=0; i < NF; i++) {
        glBegin(GL_LINES);
            for (j=0; j < 4; j++) {
                GL_VERTER_ARRAY(vertexes[faces[i][j]]);
            }
        glEnd();
    }
    
#define NC 8
    float available_colors[NC][3] ={{0, 0, 1}, 
                                    {0, 1, 0},
                                    {1, 0, 0},
                                    {1, 1, 0},
                                    {1, 0, 1},
                                    {0, 1, 1},
                                    {1, 0, 1},
                                    {1, 1, 1}};
    int colors[NF] = {0};
    for (i=0; i < NF; i++) {
        colors[i] = -1;
    }
    

    for (i=0; i < NF; i++) {
        // Colors
        short found = FALSE;
        for (j = 0; j < NC && !found; j++) {
            short inUse = FALSE;
            for (k = 0; k < NN && neighbors[i][k] != -1 && !inUse; k++) {
                if (colors[neighbors[i][k]] == j) {
                    inUse = TRUE;
                }
            }
            if (!inUse) {
                colors[i] = j;
                found = TRUE;
            } else {
            }
        }
    }
    
    for (i=0; i < NF; i++) {
//        if (i == 0) {
//            GL_COLORS_ARRAY(available_colors[NC-1]);
//        } else {
            GL_COLORS_ARRAY(available_colors[colors[i]]);
//        }
        glBegin(GL_POLYGON);
            for (j=0; j<4; j++) {
                GL_VERTER_ARRAY(vertexes[faces[i][j]]);
            }
        glEnd();
    }
}


/*
 * Game
 */

void initGame() {
    int i;
    for(i=0; i < 4; i++) {
        gamequat[i] = lastquat[i];
    }
}

/*
 * Events
 * 
 */

void onKeyPress(unsigned char key, int keyX, int keyY) {
    switch(key) {
        case '1':
            showAxis = showAxis == 0?1:0;
        break;
        case '2':
            game = game == 0?1:0;
            if (game) {
                initGame();
            }
        break;
        case 'q':
            r[0][1] = r[0][1] - angles[0];
        break;
        case 'w':
            r[0][1] += angles[0];
        break;
        case 'a':
            r[1][1] -= angles[1];
        break;
        case 's':
            r[1][1] += angles[1];
        break;
        case 'z':
            r[2][1] -= angles[2];
        break;
        case 'x':
            r[2][1] += angles[2];
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    spinning = 0;
    glutIdleFunc(NULL);
    moving = 1;
    beginx = x;
    beginy = y;
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      scaling = 1;
    } else {
      scaling = 0;
    }
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    moving = 0;
    newModel = 0;
  }
}

void motion(int x, int y) {
  if (scaling) {
    scalefactor = scalefactor * (1.0 + (((float) (beginy - y)) / H));
    beginx = x;
    beginy = y;
    newModel = 1;
    glutPostRedisplay();
    return;
  }
  if (moving) {
    trackball(lastquat,
      (2.0 * beginx - W) / W,
      (H - 2.0 * beginy) / H,
      (2.0 * x - W) / W,
      (H - 2.0 * y) / H
      );
    beginx = x;
    beginy = y;
    add_quats(lastquat, curquat, curquat);
    newModel = 1;
    glutPostRedisplay();
  }
}


/*
 * Matrixes and quarterions
 */

void recalcModelView() {
  GLfloat m[4][4];

  glPopMatrix();
  glPushMatrix();
  build_rotmatrix(m, curquat);
  glMultMatrixf(&m[0][0]);
  if (scalefactor == 1.0) {
    glDisable(GL_NORMALIZE);
  } else {
    glEnable(GL_NORMALIZE);
  }
  glScalef(scalefactor, scalefactor, scalefactor);
  newModel = 0;
}

void multiply(float m[4][4], float c[3], float r[3]) {
    float c2[4] = {c[0], c[1], c[2], 1};
    float r2[4] = {0};
    
    int x, y;
    for (x=0; x<4; x++) {
        for (y=0; y<4; y++) {
            r2[y] += m[y][x]*c2[x];
        }        
    }
    
    for (y=0; y<3; y++) {
        r[y] = r2[y] / r2[3];
    }
}

void drawRotationAxis2() {
    float colors[3][3] = {{0.5, 0.3, 0.3},
                          {0.3, 0.5, 0.3},
                          {0.3, 0.3, 0.5}
                         };
    const int n = 3;
    int i;
    int radius = 10;
    for (i = 0; i < n; i++) {
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0, 0, 0);
            glVertex3f(axis2[i][0] * radius, axis2[i][1] * radius, axis2[i][2] * radius);
            glVertex3f(axis2[i][0] * radius+0.2, axis2[i][1] * radius+0.2, axis2[i][2] * radius+0.2);
        glEnd();
    }
}

void rotate(int by, float old[3][3], float new[3][3], float phi) {
    GLfloat m[4][4];
    float q[4] = {0};
    int i,x,y;
    for (i=0; i<4; i++) {
        q[i] = 0;
    }
    axis_to_quat(old[by],RAD(phi), q);
    build_rotmatrix(m, q);
    
    printf("______  %f  __________\n", phi);
//    printf("A %f %f %f | O %f %f %f : %f %f %f\n", a[0], a[1], a[2], old[0][0], old[0][1], old[0][2], old[1][0], old[1][1], old[1][2]);
    printf("Q %f %f %f %f\n", q[0], q[1], q[2], q[3]);
    printf("Matrix:\n");
    for(y=0; y<4; y++) {
        for (x=0; x<4; x++) {
            printf("  %f", m[y][x]);
        }
        printf("\n");
    }
    for (i=0; i<3; i++) {
        if (i != by) {
            printf(">>>Multipying: %d != %d\n", i, by);
            multiply(m, old[i], new[i]);
        }
    }
    
    for (i=0; i<NV; i++) {
        multiply(m, vertexes[i], vertexes[i]);
    }
    
    printf("}[%f %f %f]\n", new[0][0], new[0][1], new[0][2]);
}

void manualRotate(float source[3], int angle, float result[3]) {
    float x = source[0];
    float y = source[1];
    float z = source[2];
    float c = cos(RAD(angle));
    float s = sin(RAD(angle));
    float m[4][4] = {{c + x*x*(1-c),    x*y*(1-c)-z*s,  x*z*(0-c)+y*s},
                     {y*z*(1-c)+z*s,    c+y*y*(1-c),    x*z*(1-c)-x*s},
                     {z*x*(1-c)-y*s,     z*y*(1-c)+x*s,  c+z*z*(1-c)}};
//    multiply(m, result, result);
}

void redraw() {
    GLfloat m[4][4];
    if (newModel) {
        recalcModelView();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int i;
//    for (i=0; i<2; i++) {
//        glRotatef(r[i][0], axis2[i][0], axis2[i][1], axis2[i][20]);
//    }
//    if (showAxis) {
//        drawRotationAxis();
//    }
//    for (i=0; i<2; i++) {
//        glRotatef(r[2-i][0], -axis2[2-i][0], -axis2[2-i][1], -axis2[2-i][20]);
    drawRotationAxis2();
//    }
  
    drawObject();
  
    glPopMatrix();
    glPushMatrix();
    build_rotmatrix(m, empty);
    glMultMatrixf(&m[0][0]);

    glTranslatef(-5, -5, -1);
    glScalef(4,4,4);
    drawAxis();
  
    /* Game */
//    if (game) {
//        glPopMatrix();
//        glPushMatrix();
//        build_rotmatrix(m, empty);
//        glTranslatef(5, 5, 0);
//        glMultMatrixf(&m[0][0]);
//        
//        glColor3f(1, 0, 1);
//        float r = 6;
//        glBegin(GL_LINE_LOOP);
//            glVertex3f(0, 0, 0);
//            glVertex3f(curquat[1] * r, curquat[2] * r, curquat[3] * r);
//        glEnd();
//    }
    
    
    glPopMatrix();
    glPushMatrix();
    
    build_rotmatrix(m, curquat);
    glMultMatrixf(&m[0][0]);
    
  glutSwapBuffers();
}

/*
 * General
 */

static void timerCallback (int value) {
    int a = 4;
    int i;
    for (i=0; i < 3; i++) {
        if (r[i][1] > r[i][0]) {
            r[i][0] += a;
            rotate(i, axis2, axis2, a);
        } else if (r[i][1] < r[i][0]) {
            r[i][0] -= a;
            rotate(i, axis2, axis2, -a);
        }
    }
//    printf("R %d->%d %d->%d %d->%d\n", r[0][0], r[0][1], r[1][0], r[1][1], r[2][0], r[2][1]);
//    printf("\tQG %f %f %f %f\n", gamequat[0], gamequat[1], gamequat[2], gamequat[3]);
//    printf("\tQC %f %f %f %f\n", curquat[0], curquat[1], curquat[2], curquat[3]);
    
    glutTimerFunc (40, timerCallback, value);
    glutPostRedisplay();
}

void myReshape(int w, int h) {
  glViewport(0, 0, w, h);
  W = w;
  H = h;
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  trackball(curquat, 0.0, 0.0, 0.0, 0.0);
  trackball(empty, 0.0, 0.0, 0.0, 0.0);
  trackball(gamequat, 0.0, 0.0, 0.0, 0.0);
  glutInitWindowSize(W,H);
  glutCreateWindow("U3");
  glutDisplayFunc(redraw);
  glutReshapeFunc(myReshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutTimerFunc (40, timerCallback, 0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
  /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 40.0);
  glMatrixMode(GL_MODELVIEW);
  glutKeyboardFunc(onKeyPress);
  gluLookAt(0.0, 0.0, 30.0,  /* eye is at (0,0,30) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
  glPushMatrix();       /* dummy push so we can pop on model recalc */
  glutMainLoop();
  
  
  return 0;
}
