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

#define KEY_ESCAPE 27
#define KEY_ENTER 13

#define FALSE 0
#define TRUE 1

int rx, ry, rz = 0;
int rrx, rry, rrz = 0;
float s=0.5;
int r = 0;
short timer = FALSE;

float rrrz=90.0, rrrx=45.0;
float vellocityZ = 0, vellocityX = 0; 
float sdepth = 10;
short leftButton = FALSE, middleButton = FALSE, leftButtonUp = FALSE;
int downX, downY, lastX, lastY;

void onKeyPress(unsigned char key, int keyX, int keyY) {
    switch (key) {
        case 'q':
            rx--;
        break;
        case 'w':
            rx++;
        break;
        case 'a':
            ry--;
        break;
        case 's':
            ry++;
        break;
        case 'z':
            rz--;
        break;
        case 'x':
            rz++;
        break;
              
        case '1':
            rrx += 180;
        break;
        case '2':
            rry += 180;
        break;
        case '3':
            rrz += 180;
        break;
        
        case KEY_ENTER:
            rx = 0;
            ry = 0;
            rz = 0;
        break;
    }
    if (rx > 360) { rx -= 360; } else if (rx < 0) { rx += 360; }
    if (ry > 360) { ry -= 360; } else if (ry < 0) { ry += 360; }
    if (rz > 360) { rz -= 360; } else if (rz < 0) { rz += 360; }
    if (rrx > 360) { rrx -= 360; } else if (rrx < 0) { rrx += 360; }
    if (rry > 360) { rry -= 360; } else if (rry < 0) { rry += 360; }
    if (rrz > 360) { rrz -= 360; } else if (rrz < 0) { rrz += 360; }
    glutPostRedisplay();
//    printf("Rot (%d, %d, %d) Translate (%d, %d, %d)\n", rx, ry, rz, tx, ty, tz);
}

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
    float phi1 = phi / 5;
    float phi2 = phi*phi / 5;
    float phi3 = phi*phi*phi / 5;
    
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
//        printf("c[%d]:\n", i);
        for (j = 0; j < NC && !found; j++) {
//            printf("\tj=%d found=%d\n", j, found);
            short inUse = FALSE;
            for (k = 0; k < NN && neighbors[i][k] != -1 && !inUse; k++) {
//                printf("\t\tn[%d][%d]  c[%d]=%d\n", i, k, neighbors[i][k], colors[neighbors[i][k]]);
                if (colors[neighbors[i][k]] == j) {
                    inUse = TRUE;
                }
//                printf("\t\t n[%d][%d] c[%d]=%d inUse=%d\n", i, k, neighbors[i][k], colors[neighbors[i][k]], inUse);
            }
            if (!inUse) {
                colors[i] = j;
//                printf("\t\t\t c[%d] = %d\n", i, j);
                found = TRUE;
            } else {
//                printf("\t\t\t Cont c[%d] = %d\n", neighbors[i][k], colors[neighbors[i][k]]);
            }
        }
    }
    
    for (i=0; i < NF; i++) {
//        printf("c[%d] %d\n", i, colors[i]);
    }
    
    for (i=0; i < NF; i++) {
        GL_COLORS_ARRAY(available_colors[colors[i]]);
        glBegin(GL_POLYGON);
            for (j=0; j<4; j++) {
                GL_VERTER_ARRAY(vertexes[faces[i][j]]);
            }
        glEnd();
    }
    
//    exit(0);
}


GLvoid Timer( int value )
{
//    if (timer) {
//        rx += 2;
//        ry += 4;
//        rz += 6;
//        glutPostRedisplay();
//    }
//    vellocityX /= 2;
//    vellocityZ /= 2;
    
    rrrx += vellocityX;
    rrrz += vellocityZ;
    vellocityX /= 1.1;
    vellocityZ /= 1.1;
    glutPostRedisplay();
    
    glutTimerFunc(40,Timer,value);
}

/*
 * Main drawing loop
 */
void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glScalef(s, s, s);
    glRotatef(rx, 1.0, 0, 0);
    glRotatef(ry, 0, 1.0, 0);
    glRotatef(rz, 0, 0, 1.0);

    if (rrrz > 360) {
        rrrz -= 360;
    }
    if (rrrx > 360) {
        rrrx -= 360;
    }
    if (rrrz < 0) {
        rrrz += 360;
    }
    if (rrrx < 0) {
        rrrx += 360;
    }
    glRotatef(rrrx, 1.0, 0.0, 0.0);
    glRotatef(rrrz, 0.0, 0.0, 1.0);
    
    glRotatef(rrx, 1.0, 0, 0);
    glRotatef(rry, 0, 1.0, 0);
    glRotatef(rrz, 0, 0, 1.0);
    
    drawObject();    

    glTranslatef(-1, -1, -1);
    drawAxis();
    
    glutSwapBuffers();
}

/*
 * Mouse 
 */

void MouseCallback(int button, int state, int x, int y) {
    leftButtonUp = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP));
    if (leftButtonUp) {
        vellocityX+=(float)(lastY-y)/4.0;
        if (rrrx < 180) {
            vellocityZ+=(float)(x-lastX)/4.0;
        } else {
            vellocityZ-=(float)(x-lastX)/4.0;
        }
    } else {
        vellocityX /= 4;
        vellocityZ /= 4;
    }

    downX = x; downY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) &&  (state == GLUT_DOWN));
    glutPostRedisplay();
}

    
void MotionCallback(int x, int y) {
    if (leftButton) {
        rrrx+=(float)(downY-y)/4.0;
        if (rrrx < 180) {
          rrrz+=(float)(x-downX)/4.0;
        } else {
            rrrz-=(float)(x-downX)/4.0;
        }
        vellocityX /= 4;
        vellocityZ /= 4;
    }
    
    lastX = downX;
    lastY = downY;
    downX = x;   downY = y; 
    glutPostRedisplay();
}


/*
 * Initiating OpenGl
 */
int main(int argc, char **argv) {
    // Init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100,400);
    glutInitWindowSize(640,640);
    glutCreateWindow("U3");    
    glEnable(GL_DEPTH_TEST);
    
    // Register callbacks
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(onKeyPress);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
    glutTimerFunc(40,Timer,0);
    
    // Enter GLUT event processing cycle
    glutMainLoop();
    

    return 0;
}
