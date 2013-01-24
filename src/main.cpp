/**
 * @author Aurelijus Banelis
 */

#include <GL/gl.h>
#include <GL/glut.h>

#include "Scene.h"

Scene *scene;

void draw() {
    scene->draw();
}

void onKeyPress(unsigned char key, int keyX, int keyY) {
    scene->onKeyPress(key, keyX, keyY);
}

void onMousePress(int button, int state, int x, int y) {
    scene->onMousePress(button, state, x, y);
}

void onMouseMove(int x, int y) {
    scene->onMouseMove(x, y);
}

void reshape(int w, int h) {
    scene->reshape(w, h);
}

void timer(int value) {
    scene->timer(value);
    glutTimerFunc (40, timer, value);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    scene = new Scene();
    scene->init();
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(onMousePress);
    glutMotionFunc(onMouseMove);
    glutTimerFunc (40, timer, 0);
    glutKeyboardFunc(onKeyPress);
    glutMainLoop();
    delete scene;
    return 0;
}

