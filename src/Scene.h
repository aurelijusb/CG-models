/**
 * @author Aurelijus Banelis
 */

#ifndef SCENE_H
#define	SCENE_H

#include <iostream>
#include <vector>
#include "Objects3D.h"
#include <GL/gl.h>

using namespace std;
using namespace Objects3D;

class Scene {
public:
    Scene();
    
    void init();
    void draw();
    void onKeyPress(unsigned char key, int keyX, int keyY);
    void onMousePress(int button, int state, int x, int y);
    void onMouseMove(int x, int y);
    void reshape(int w, int h);
    void timer(int value);
    
    virtual ~Scene();
private:
    vector<Object3D*> models;
    vector<Material*> materials;
    GLfloat *lightAmbient;
    GLfloat *lightDiffuse;
    GLfloat *lightPosition;
};

#endif	/* SCENE_H */

