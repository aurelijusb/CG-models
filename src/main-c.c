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
#include "normals.h"
#include "main.h"

#define PI 3.141592653589793
#define RAD(angle) (angle * PI / 180.0)
#define SIN_21 0.3583679495453
#define COS_21 0.9335804264972
#define SIN_58 0.8480480961564
#define COS_58 0.5299192642332
#define DEG(angle) (angle * 180 / PI);

GLfloat axis[3][3] = {{0, 1, 0},
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
short showAxis = FALSE;
short game = 0;
short lighting = TRUE;
short skeleton = TRUE;
short materials = TRUE;

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

#define NC 8
    float available_colors[NC][3] ={{0, 0, 1}, 
                                    {0, 1, 0},
                                    {1, 0, 0},
                                    {1, 1, 0},
                                    {1, 0, 1},
                                    {0, 1, 1},
                                    {1, 0, 1},
                                    {1, 1, 1}};

float mat_red[4] = {0.5, 0.1, 0.1, 0.5};
float mat_green[4] = {0.1, 0.5, 0.1, 0.5};
float mat_blue[4] = {0.1, 0.1, 0.5, 0.5};
float mat_gray[4] = {0.5, 0.5, 0.5, 0.5};
    
float exludedVertex[4][3] = {0};
float originalVertex[4][3] = {0};
int exluded = TRUE;

int myX = -1;
int myY = -1;
int myZ = -1;

char *sourceFile = "bezdzione.obj";

#define KEY_ESCAPE 27

/*
 * Loading external models
 */

void loadModel(char *file) {
    
}

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

void normals(int face) {
    int i, j;
    float normal[3];
    for (i=0; i < 4; i++) {
        for(j=0; j < 3; j++) {
            normal[j] += vertexes[faces[face][i]][j];
        }
    }
    for(j=0; j < 3; j++) {
        normal[j] /= 4;
    }
    glNormal3fv(normal);
}

void drawObject(float vertices[NV][3], int exclude) {
    int i, j, k, l, m;
    
    float phi = (1+sqrt(5))/2;
    float phi1 = phi;
    float phi2 = phi*phi;
    float phi3 = phi*phi*phi;  
    
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
            }
        }
    }
    
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_gray);
    }
    
    glEnable(GL_NORMALIZE);
    GLfloat fNormalX, fNormalY, fNormalZ;
    for (i=0; i < NF; i++) {
        if (i != exclude) {
            CalculateVectorNormal(vertexes[faces[i][0]], vertexes[faces[i][1]],
                     vertexes[faces[i][2]], &fNormalX, &fNormalY,
                     &fNormalZ);
            glNormal3f(-fNormalX, -fNormalY, -fNormalZ);
            glColor3fv(available_colors[colors[i]]);
            glBegin(GL_POLYGON);
                for (j=0; j<4; j++) {
                    glVertex3fv(vertexes[faces[i][j]]);
                }
            glEnd();
        }
    }
    glDisable(GL_NORMALIZE);
}

void drawExcluded() {
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
    }
    
    glColor3fv(available_colors[NC-1]);
    GLfloat fNormalX, fNormalY, fNormalZ;
    CalculateVectorNormal(exludedVertex[1], exludedVertex[2],
                         exludedVertex[3], &fNormalX, &fNormalY,
                         &fNormalZ);
    
    glNormal3f(-fNormalX, -fNormalY, -fNormalZ);
    glEnable(GL_NORMALIZE);
    glBegin(GL_POLYGON);
        int i;
        for (i=0; i<4; i++) {
            glVertex3fv(exludedVertex[i]);
        }
    glEnd();
    glDisable(GL_NORMALIZE);
    
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_green);
    }
    
    glBegin(GL_LINE_LOOP);
        for (i=0; i<4; i++) {
            glVertex3fv(originalVertex[i]);
        }
    glEnd();
    
}


void drawCylinder(float radius, float height, int slices) {
    float rad;
    float next = PI * 2 / slices;
    for (rad = 0; rad < PI * 2; rad += next) {
        float z1 = sin(rad) * radius;
        float z2 = sin(rad + next) * radius;
        float x1 = cos(rad) * radius;
        float x2 = cos(rad + next) * radius;
        float y1 = -height / 2;
        float y2 = height / 2;
        float nz = sin(rad + (next / 2));
        float nx = cos(rad + (next / 2));
        glNormal3f(nx, 0, nz);
        glBegin(GL_QUADS);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y1, z2);
            glVertex3f(x2, y2, z2);
            glVertex3f(x1, y2, z1);
        glEnd();
    }
}

void rotateBy(float axis[3], float phi) {
    GLfloat m[4][4];
    float q[4] = {0};
    axis_to_quat(axis, RAD(phi), q);
    build_rotmatrix(m, q);
    glMultMatrixf(&m[0][0]);
}

float getAngle(float v1[3], float v2[3]) {
    float len1 = sqrt(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2]);
    float len2 = sqrt(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2]);
    float dotprd = (v1[0]*v2[0])+(v1[1]*v2[1])+(v1[2]*v2[2]);
    return acos(dotprd/(len1*len2));
}

void drawSkeleton() {
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
    }

    int i, j, k;
    for(i = 0; i < NV; i++) {
        glPushMatrix();
            glTranslatef(vertexes[i][0], vertexes[i][1], vertexes[i][2]);
            glutSolidSphere(0.2, 32, 16);
        glPopMatrix();
    }
    
    float center[3];
    float difference[3];
    
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_green);
    }
    
    float cylinder[3] = {0, 1, 0};
    float zero[3] = {0, 0, 0};
    for(i = 0; i < NV; i++) {
        for (j = 0; j < 4; j++) {
            int j2 = j < 3 ? j + 1 : 0;
            float distance = 0;
            for (k = 0; k < 3; k++) {
                difference[k] = vertexes[faces[i][j]][k] - vertexes[faces[i][j2]][k];
                distance += difference[k] * difference[k];
                center[k] = (vertexes[faces[i][j]][k] + vertexes[faces[i][j2]][k]) / 2;
            }
            
            glPushMatrix();
                glTranslatef(center[0], center[1], center[2]);

                /* Normalizing */
                difference[0] /= sqrt(distance);
                difference[1] /= sqrt(distance);
                difference[2] /= sqrt(distance);
                
                float nx, ny, nz;
                float normal[3];
                CalculateVectorNormal(zero, cylinder, difference,
                                      &nx, &ny, &nz);
                normal[0] = nx;
                normal[1] = ny;
                normal[2] = nz;
                
                float angle = DEG(getAngle(cylinder, difference));
                rotateBy(normal, angle);
                drawCylinder(0.05, sqrt(distance), 64);
            glPopMatrix();
        }
    }
    
    if (materials) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_gray);
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
}

void resetModelView() {
  GLfloat m[4][4];
  glPopMatrix();
  glPushMatrix();
  build_rotmatrix(m, empty);
  glMultMatrixf(&m[0][0]);
  if (scalefactor == 1.0) {
    glDisable(GL_NORMALIZE);
  } else {
    glEnable(GL_NORMALIZE);
  }
  glScalef(scalefactor, scalefactor, scalefactor);
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

float *getMaterial(int i) {
    switch(i) {
        case 0:
            return mat_red;
        case 1:
            return mat_green;
        case 2:
            return mat_blue;
        default:
            return mat_gray;
    }
}

void drawRotationAxis() {
    float colors[3][3] = {{0.5, 0.3, 0.3},
                          {0.3, 0.5, 0.3},
                          {0.3, 0.3, 0.5}
                         };
    
    const int n = 3;
    int i;
    int radius = 10;
    for (i = 0; i < n; i++) {
        if (materials) {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, getMaterial(i));
        }
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0, 0, 0);
            glVertex3f(axis[i][0] * radius, axis[i][1] * radius, axis[i][2] * radius);
        glEnd();
    }
}

void rotate(int by, float axis[3][3], int phi) {
    GLfloat m[4][4];
    float q[4] = {0};
    int i;
    for (i=0; i<4; i++) {
        q[i] = 0;
    }
    axis_to_quat(axis[by],RAD(phi), q);
    build_rotmatrix(m, q);
    
    for (i=0; i<3; i++) {
        if (i != by) {
            multiply(m, axis[i], axis[i]);
        }
    }
    
    for (i=0; i<NV; i++) {
        multiply(m, vertexes[i], vertexes[i]);
    }
    for (i=0; i<4; i++) {
        multiply(m, originalVertex[i], originalVertex[i]);
    }
}

/*
 * Lighting
 */

void initLight() {
    GLfloat light0_specular[] = { 1.0, 0.8, 0.8, 1.0 };
    GLfloat light0_shininess[] = { 50.0 };
    GLfloat light0_position[] = { 10.0, 10.0, 10.0, 0.0 };
    glMaterialfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glMaterialfv(GL_LIGHT0, GL_SHININESS, light0_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    GLfloat light1_ambient[] = { 0.4, 0.4, 0.6, 1.0 };
    GLfloat light1_diffuse[] = { 0.0, 0.0, 1, 1.0 };
    GLfloat light1_specular[] = { 0.6, 0.8, 1.0, 1.0 };
    GLfloat light1_position[] = { -1.0, 0.0, 0.0, 0.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);
    
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

/*
 * Game
 */

void initGame() {
    int i, j;
    float x = 0;
    float y = 0;
    float z = 0;
    for (i=0; i<4; i++) {
        for (j=0; j<3; j++) {
            exludedVertex[i][j] = vertexes[faces[exluded][i]][j];
        }
        x += exludedVertex[i][0];
        y += exludedVertex[i][1];
        z += exludedVertex[i][2];
    }
    x /= 3;
    y /= 3;
    z /= 3;
    for (i=0; i<4; i++) {
        exludedVertex[i][0] += x / 6;
        exludedVertex[i][1] += y / 6;
        exludedVertex[i][2] += z / 6;
        for (j=0; j < 3; j++) {
            originalVertex[i][j] = exludedVertex[i][j];
        }
    }
    
    printf("Randomized\n");
    for (i=0; i<2; i++) {
        j = rand() % 3;
        rotate(j, axis, angles[j]);
        printf("\tAxis %d by %d degrees\n", j, angles[j]);
    }
}

void finishGame() {
    game = FALSE;
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
        case '3':
            lighting = lighting == 0?1:0;
            printf("Lighting: %d\n", lighting);
            if (lighting) {
                glEnable(GL_LIGHTING);
            } else {
                glDisable(GL_LIGHTING);
            }
        break;
        case '4':
            skeleton = skeleton == 0?1:0;
        break;
        case '5':
            materials = materials == 0?1:0;
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
        case KEY_ESCAPE:
            exit(0);
        break;
        case 'e':
            myX-=1;
        break;
        case 'r':
            myX+=1;
        break;
        case 'd':
            myY-=1;
        break;
        case 'f':
            myY+=1;
        break;
        case 'c':
            myZ-=1;
        break;
        case 'v':
            myZ+=1;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        spinning = 0;
        glutIdleFunc(NULL);
        moving = 1;
        beginx = x;
        beginy = y;
        if (glutGetModifiers() & GLUT_ACTIVE_SHIFT && state == GLUT_DOWN) {
            scaling = 1;
        }
    } else if (button == 4) {
        scalefactor *= 0.9;
    } else if (button == 3) {
        scalefactor *= 1.1;
    } else {
        scaling = 0;
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

void redraw() {
    GLfloat m[4][4];
    recalcModelView();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (showAxis) {
        drawRotationAxis();
    }
    
    if (game) {
        drawObject(vertexes, exluded);
        drawExcluded();
    } else {
        drawObject(vertexes, -1);
    }

    if (skeleton) {
        drawSkeleton();
    }
      
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
            rotate(i, axis, a);
        } else if (r[i][1] < r[i][0]) {
            r[i][0] -= a;
            rotate(i, axis, -a);
        }
    }
    
    if (game) {
        short found = TRUE;
        float approximate = 0.004;
        int i, j;
        for (i=0; i<4 && found; i++) {
            for (j=0; j < 3 && found; j++) {
                float difference = abs(originalVertex[i][j] - exludedVertex[i][j]);
                if (difference > approximate) {
                    found = FALSE;
                }
            }
        }
        if (!found) {
            found = TRUE;            
            for (i=0; i<4 && found; i++) {
                for (j=0; j < 3 && found; j++) {
                    int i2 = i > 1 ? i - 2 : i + 2;
                    float difference = abs(originalVertex[i][j] - exludedVertex[i2][j]);
                    if (difference > approximate) {
                        found = FALSE;
                    }
                }
            }
        }
        if (found) {
            finishGame();
        }
    }
    
    glutTimerFunc (40, timerCallback, value);
    glutPostRedisplay();
}

void myReshape(int w, int h) {
  glViewport(0, 0, w, h);
  W = w;
  H = h;
}

int main2(int argc, char **argv) {
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
  
  //FIXME: debug info
  if (game) {
      initGame();
  }
  if (lighting) {
      initLight();
  }
  
  glutMainLoop();
  
  
  return 0;
}
