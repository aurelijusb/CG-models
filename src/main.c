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

GLfloat angle = -150;   /* in degrees */
GLboolean doubleBuffer = GL_TRUE, iconic = GL_FALSE, keepAspect = GL_FALSE;
int spinning = 0, moving = 0;
int beginx, beginy;
int W = 600, H = 600;
float empty[4];
float curquat[4];
float lastquat[4];
GLdouble bodyWidth = 3.0;
int newModel = 1;
int scaling;
float scalefactor = 1.0;
int rx1, rx2 = 0;
int ry1, ry2 = 0;
int rz1, rz2 = 0;
short showAxis = 0;

/*
 * Models
 */

void drawAxis() {
    // X (by y)
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0.8, 0);
        glVertex3f(0.2, 0.6, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(-0.2, 0.6, 0);
        glVertex3f(0, 0.8, 0);
    glEnd();
    
    
    //Y (by x)
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0.8, 0, 0);
        glVertex3f(0.6, 0.2, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(0.6, -0.2, 0);
        glVertex3f(0.8, 0, 0);
    glEnd();
    
    
    //Z (by XY)
    glColor3f(0.2, 0.2, 1);
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 0.8);
        glVertex3f(0.2, 0.2, 0.6);
        glVertex3f(0, 0, 1);
        glVertex3f(-0.2, -0.2, 0.6);
        glVertex3f(0, 0, 0.8);
    glEnd();
}

void drawObject() {
    int i, j, k, l, m;
    
    float phi = (1+sqrt(5))/2;
    float phi1 = phi;
    float phi2 = phi*phi;
    float phi3 = phi*phi*phi;
    
#define NV 32
    float vertexes[NV][3] = {{phi2, phi2, phi2}, 
                            {phi2, phi2, -phi2}, 
                            {phi2, -phi2, phi2}, 
                            {phi2, -phi2, -phi2}, 
                            {-phi2, phi2, phi2}, 
                            {-phi2, phi2, -phi2}, 
                            {-phi2, -phi2, phi2}, 
                            {-phi2, -phi2, -phi2}, 
                            {phi1, phi3, -0.00}, 
                            {-phi1, phi3, -0.00}, 
                            {phi1, -phi3, -0.00}, 
                            {-phi1, -phi3, 0.000}, 
                            {phi3, -0.00, phi1}, 
                            {phi3, -0.00, -phi1}, 
                            {-phi3, -0.00, phi1}, 
                            {-phi3, 0.000, -phi1}, 
                            {-0.00, phi1, phi3}, 
                            {-0.00, -phi1, phi3}, 
                            {-0.00, phi1, -phi3}, 
                            {0.000, -phi1, -phi3}, 
                            {-0.00, phi3, phi2}, 
                            {phi3, phi2, -0.00}, 
                            {phi2, -0.00, phi3}, 
                            {-0.00, phi3, -phi2}, 
                            {phi3, -phi2, -0.00}, 
                            {-phi2, -0.00, phi3}, 
                            {-phi3, phi2, -0.00}, 
                            {-0.00, -phi3, phi2}, 
                            {phi2, -0.00, -phi3}, 
                            {-phi2, -0.00, -phi3}, 
                            {-phi3, -phi2, -0.00}, 
                            {-0.00, -phi3, -phi2}};   
    
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
        GL_COLORS_ARRAY(available_colors[colors[i]]);
        glBegin(GL_POLYGON);
            for (j=0; j<4; j++) {
                GL_VERTER_ARRAY(vertexes[faces[i][j]]);
            }
        glEnd();
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
            printf("%d\n", showAxis);
        break;
        case 'q':
            rx2 -= 180;
        break;
        case 'w':
            rx2 += 180;
        break;
        case 'a':
            ry2 -= 180;
        break;
        case 's':
            ry2 += 180;
        break;
        case 'z':
            rz2 -= 180;
        break;
        case 'x':
            rz2 += 180;
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

void redraw() {
  GLfloat m[4][4];
    
  if (newModel)
    recalcModelView();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glRotatef(rx1, 1, 0, 0);
  glRotatef(ry1, 0, 1, 0);
  glRotatef(rz1, 0, 0, 1);
  drawObject();
  if (showAxis) {
      glScalef(10, 10, 10);
      drawAxis();
  }
  
    glPopMatrix();
    glPushMatrix();
    build_rotmatrix(m, empty);
    glMultMatrixf(&m[0][0]);

    glTranslatef(-5, -5, -1);
    glScalef(4,4,4);
    drawAxis();
  
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
//    if (rx1 >= 360) {
//        rx1 -= 360;
//    }
//    if (ry1 >= 360) {
//        ry1 -= 360;
//    }
//    if (rz1 >= 360) {
//        rz1 -= 360;
//    }
//    if (rx1 < 0) {
//        rx1 += 360;
//    }
//    if (ry1 <= 360) {
//        ry1 += 360;
//    }
//    if (rz1 <= 360) {
//        rz1 += 360;
//    }
//    if (rx2 >= 360) {
//        rx2 -= 360;
//    }
//    if (ry2 >= 360) {
//        ry2 -= 360;
//    }
//    if (rz2 >= 360) {
//        rz2 -= 360;
//    }
//    if (rx2 < 0) {
//        rx2 += 360;
//    }
//    if (ry2 <= 360) {
//        ry2 += 360;
//    }
//    if (rz2 <= 360) {
//        rz2 += 360;
//    }
    int a = 5;
    if (rx2 > rx1) {
        rx1 += a;
    } else if (rx2 < rx1) {
        rx1 -= a;
    }
    if (ry2 > ry1) {
        ry1 += a;
    } else if (ry2 < ry1) {
        ry1 -= a;
    }
    if (rz2 > rz1) {
        rz1 += a;
    } else if (rz2 < rz1) {
        rz1 -= a;
    }
    printf("%d, %d | %d, %d | %d, %d\n", rx1, rx2, ry1, ry2, rz1, rz2);
    
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
