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

int rx, ry, rz = 0;
int tz = 0;
int tx = -1;
int ty = -1;
float s=0.25;
int r = 0;

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
        
        case 'e':
            tx--;
        break;
        case 'r':
            tx++;
        break;
        case 'd':
            ty--;
        break;
        case 'f':
            ty++;
        break;
        case 'c':
            tz--;
        break;
        case 'v':
            tz++;
        break;
        
        case KEY_ENTER:
            rx = 0;
            ry = 0;
            rz = 0;
            tx = 0;
            ty = 0;
            tz = 0;
        break;
        case '1':
            s *= 2;
        break;
        case '2':
            s /= 2;
        break;
    }
    if (rx > 360) { rx -= 360; } else if (rx < 0) { rx += 360; }
    if (ry > 360) { ry -= 360; } else if (ry < 0) { ry += 360; }
    if (rz > 360) { rz -= 360; } else if (rz < 0) { rz += 360; }
    if (r > 360) { r -= 360; } else if (r < 0) { r += 360; }
    glutPostRedisplay();
    printf("Rot (%d, %d, %d) Translate (%d, %d, %d)\n", rx, ry, rz, tx, ty, tz);
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
    
//        # Calculate the necessary constants
//        s = 1/sqrt(3)
//        t = sqrt((3-sqrt(5))/6)
//        u = sqrt((3+sqrt(5))/6)
//
//        # create the vertices and faces
//        v = [(s,s,s),(s,s,-s),(s,-s,s),(s,-s,-s),(-s,s,s),(-s,s,-s),(-s,-s,s),(-s,-s,-s),
//             (t,u,0),(-t,u,0),(t,-u,0),(-t,-u,0),(u,0,t),(u,0,-t),(-u,0,t),(-u,0,-t),(0,t,u),
//             (0,-t,u),(0,t,-u),(0,-t,-u)]
//        faces = [[0,8,9,4,16],[0,12,13,1,8],[0,16,17,2,12],[8,1,18,5,9],[12,2,10,3,13],
//                 [16,4,14,6,17],[9,5,15,14,4],[6,11,10,2,17],[3,19,18,1,13],[7,15,5,18,19],
//                 [7,11,6,14,15],[7,19,3,10,11]]

//>>> for v in o.data.vertices:
//...     print ("{%f, %f, %f}, " % (v.co[0], v.co[1], v.co[2]))
    
#define NV 32
    float vertexes[NV][3] = {{0.525731, 0.525731, 0.525731}, 
                            {0.525731, 0.525731, -0.525731}, 
                            {0.525731, -0.525731, 0.525731}, 
                            {0.525731, -0.525731, -0.525731}, 
                            {-0.525731, 0.525731, 0.525731}, 
                            {-0.525731, 0.525731, -0.525731}, 
                            {-0.525731, -0.525731, 0.525731}, 
                            {-0.525731, -0.525731, -0.525731}, 
                            {0.324920, 0.850651, -0.000000}, 
                            {-0.324920, 0.850651, -0.000000}, 
                            {0.324920, -0.850651, -0.000000}, 
                            {-0.324920, -0.850651, 0.000000}, 
                            {0.850651, -0.000000, 0.324920}, 
                            {0.850651, -0.000000, -0.324920}, 
                            {-0.850651, -0.000000, 0.324920}, 
                            {-0.850651, 0.000000, -0.324920}, 
                            {-0.000000, 0.324920, 0.850651}, 
                            {-0.000000, -0.324920, 0.850651}, 
                            {-0.000000, 0.324920, -0.850651}, 
                            {0.000000, -0.324920, -0.850651}, 
                            {-0.000000, 0.850651, 0.525731}, 
                            {0.850651, 0.525731, -0.000000}, 
                            {0.525731, -0.000000, 0.850651}, 
                            {-0.000000, 0.850651, -0.525731}, 
                            {0.850651, -0.525731, -0.000000}, 
                            {-0.525731, -0.000000, 0.850651}, 
                            {-0.850651, 0.525731, -0.000000}, 
                            {-0.000000, -0.850651, 0.525731}, 
                            {0.525731, -0.000000, -0.850651}, 
                            {-0.525731, -0.000000, -0.850651}, 
                            {-0.850651, -0.525731, -0.000000}, 
                            {-0.000000, -0.850651, -0.525731}};    
//>>> for f in o.data.faces:
//...     print ("{%d, %d, %d, %d}, " % (f.vertices[0], f.vertices[1], f.vertices[2], f.vertices[3]))
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

    
#define GL_VERTER_ARRAY(array) glVertex3f(array[0], array[1], array[2]) 
    
    //Vertexes
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
        int i;
        for (i = 0; i < NV; i++) {
            GL_VERTER_ARRAY(vertexes[i]);
        }
    glEnd();
    
    glColor3f(0, 1, 1);
    for (i=0; i < NF; i++) {
        glBegin(GL_LINES);
            int j;
            for (j=0; j < 4; j++) {
                GL_VERTER_ARRAY(vertexes[faces[i][j]]);
            }
        glEnd();
    }
}



/*
 * Main drawing loop
 */
void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glScalef(s, s, s);
    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1.0, 0, 0);
    glRotatef(ry, 0, 1.0, 0);
    glRotatef(rz, 0, 0, 1.0);
    drawAxis();
        
    drawObject();
    
    glutSwapBuffers();
}


/*
 * Initiating OpenGl
 */
int main(int argc, char **argv) {
    // Init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA);
    glutInitWindowPosition(100,400);
    glutInitWindowSize(640,640);
    glutCreateWindow("Tests");    
    
    // Register callbacks
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(onKeyPress);
    
    // Enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
