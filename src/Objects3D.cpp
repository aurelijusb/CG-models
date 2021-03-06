/**
 * @author Aurelijus Banelis
 */

#include <fstream>
#include <iostream>
#include <GL/gl.h>
#include <stdlib.h>
#include "Objects3D.h"

using namespace std;
using namespace Objects3D;

/**
 * 3D model
 * 
 * @param name                      name of object
 * @param vertixes                  vertix corrdinates. array[0..nv*3] = {x1, y1, z1, x2, y2, z2, ...} as array[vertex][coordinate]
 * @param nv                        number of different vertixes. (1 vertex = array(x, y, z) )
 * @param facesNormals              faces normals. array[0..nfn*3] = {x1, y1, z1, x2, y2, z2, ...}
 * @param nfn                       number of different face's norlmals. (1 face = array(x, y, z) )
 * @param triangles                 model trinangles. array[0..nf*3] = {I[0], I[1], I[2], ...} as array[face][element], where {I[0], I[1], I[2]} - triangle; I[i] index of vertex coordinate from vertixes array.
 * @param nf                        number of model trinagels (face per trinagle)
 * @param normals                   model faces' normals. array[0..nf] = {I[0], ...}, where I[i] index of normal from facesNormals array.
 * @param material                  material of model
 * @param freeVertixDataOnDestruct  if true, vertixes, facesNormals, faces and normals will be freed on object destruction
 * 
 *              triangle         } nf*3  ~~~~~  normal      } nf
 *              /   |   \                         |                     } vertex = trinagle[index]
 *         vertex vertex vertex  } nv         facesNormal   } nfn
 *         / | \  / | \  / | \                  / | \                   } x = vertex[index * 3 + 0]
 *         x y z  x y z  x y z   } nv*3         x y z       } nfn*3
 */
Object3D::Object3D(const string &name, float *vertixes, int nv, float *facesNormals, int nfn, int *triangles, int nf, int *normals, Material *material, bool freeVertixDataOnDestruct) {
    this->modelName = name;
    this->vertixes = vertixes;
    this->nv = nv;
    this->facesNormals = facesNormals;
    this->nfn = nfn;
    this->triangles = triangles;
    this->nf = nf;
    this->normals = normals;
    this->material = material;
}

const string &Object3D::name() {
    return modelName;
}

void Object3D::render() {
    if (material) {
        material->apply();
    }
    glEnable(GL_NORMALIZE);
    glBegin(GL_TRIANGLES);
    int vertexKey, normalsKey;
    for (int faceKey = 0; faceKey < nf; faceKey++) {
        if (nfn) {
            normalsKey = normals[faceKey];
            glNormal3fv(&facesNormals[normalsKey * 3]);
        }
        for (int i = 0; i < 3; i++) {
            vertexKey = triangles[faceKey * 3 + i];
            glVertex3fv(&vertixes[vertexKey * 3]);
        }
    }
    glEnd();
    glDisable(GL_NORMALIZE);
}

Object3D::~Object3D() {
    if (freeVertixDataOnDestruct) {
        delete  vertixes;
        delete  facesNormals;
        delete  triangles;
        delete  normals;
    }
}


void Objects3D::importFromObj(const string directory, const string fileName, vector<Object3D*> &objectStorage, vector<Material*> &materialStorage) {
    /* Opening file */
    string fullFileName = directory + fileName;
    ifstream file;   
    file.open(fullFileName.c_str(), ios::in | ios::binary);
    string buffer, buffer2;

    /* Getting counts and textual properties */
    int nV, nFN, nF, nT;
    nV = nFN = nF = nT = 0;
    ios::pos_type dataStart = 0;
    string materialFileName = "";
    string modelName = "";
    string materialName = "";
    while (!file.eof()) {
        getline(file, buffer, ' ');
        if (buffer == "mtllib") {
            getline(file, buffer);
            materialFileName = buffer;
        } else if (buffer == "usemtl") {
            getline(file, buffer);
            materialName = buffer;
        } else if (buffer == "o") {
            getline(file, buffer);;
            modelName = buffer;
            dataStart = file.tellg();
        } else if (buffer == "v") {
            nV++;
            getline(file, buffer);
        } else if (buffer == "vn") {
            nFN++;
            getline(file, buffer);
        } else if (buffer == "vt") {
            nT++;
            getline(file, buffer);
        } else if (buffer == "f") {
            nF++;
            getline(file, buffer);
        } else {
            getline(file, buffer);
        }
    }
    float *vertices = new float[nV * 3];            // vertextIndex * (x, y, z)
    float *facesNormals = new float[nFN * 3];       // normalIndex * (x, y, z)
//    float *facesTextures = new float[nT * 2];       // normalIndex * (x, y, z)
    int *trinagles = new int[nF * 3];               // tringaleIndex * (vertex1, vertex2, vertex3)
    int *normals = new int[nF];                     // normal per triangle-face
//    int *texture = new int[nF];                     // texture per triangle-face
    
    /* Reading vertex and normals coordinates */
    file.clear();
    file.seekg(dataStart);
    int iV, iFN, iF, iT;
    char tmp;
    iV = iFN = iF = iT = 0;
    
    cout << "V:" << nV << " T:" << nT << " N:" << nFN << " F:" << nF << endl;
    while (getline(file, buffer, ' ')) {
        if (buffer == "v") {
            file >> vertices[iV*3] >> vertices[iV*3 + 1] >> vertices[iV*3 + 2];
            iV++;
        } else if (buffer == "vn") {
            file >> facesNormals[iFN*3] >> facesNormals[iFN*3 + 1] >> facesNormals[iFN*3 + 2];
            iFN++;
//        } else if (buffer == "vt") {
//            file >> facesTextures[iT*2] >> facesTextures[iT*2 + 1];
//            iT++;
        } else if (buffer == "f") {
//            if (!nT && !nFN) {
//                for (int i=0; i < 3; i++) {
//                    file >> trinagles[iF * 3 + i];
//                    trinagles[iF * 3 + i]--;
//                    RANGE(trinagles[iF * 3 + i], 0, nV -1);
//                    texture[iF * 3 + i] = 0;
//                }
//                normals[iF] = 0;
//            } else if (nT && !nFN) {
//                for (int i=0; i < 3; i++) {
//                    file >> trinagles[iF * 3 + i] >> tmp >> texture[iF * 3 + i];
//                    trinagles[iF * 3 + i]--;
//                    texture[iF * 3 + i]--;
//                    RANGE(trinagles[iF * 3 + i], 0, nV -1);
//                    RANGE(texture[iF * 3 + i], 0, nT -1);
//                }
//                normals[iF] = 0;
//            } else if (!nT && nFN) {
                for (int i=0; i < 3; i++) {
                    file >> trinagles[iF * 3 + i] >> tmp >> tmp >> normals[iF];
                    trinagles[iF * 3 + i]--;
                    RANGE(trinagles[iF * 3 + i], 0, nV -1);
                    if (tmp != '/') {
                        cout << "OBJ file must have normals" << endl;
                        return;
                    }
//                    texture[iF * 3 + i] = 0;
                }
                normals[iF]--;
                RANGE(normals[iF], 0, nFN -1);
//            } else {
//                for (int i=0; i < 3; i++) {
//                    file >> trinagles[iF * 3 + i] >> tmp >> texture[iF * 3 + i] >> tmp >> normals[iF];
//                    trinagles[iF * 3 + i]--;
//                    texture[iF * 3 + i]--;
//                    RANGE(trinagles[iF * 3 + i], 0, nV -1);
//                    RANGE(texture[iF * 3 + i], 0, nT -1);
//                }
//                normals[iF]--;
//                RANGE(normals[iF], 0, nFN -1);
//            }
//            for (int i=0; i < 3; i++) {
//                cout << "{" << trinagles[iF * 3 + i] << '|' << texture[iF * 3 + i] << '|' << normals[iF] << "}\t";
//            }
//            cout << endl;
            iF++;
        }
        getline(file, buffer);
    }
    file.close();
    
    importFromMtl(directory, materialFileName, materialStorage);
    Material *material = getMaterialByName(materialStorage, materialName);

    Object3D *model = new Object3D(modelName, vertices, nV, facesNormals, nFN, trinagles, nF, normals, material);
    objectStorage.push_back(model);

    //TODO:
//        delete facesTextures;
//    delete texture;
}

//TODO: many materials in one file
void Objects3D::importFromMtl(const string directory, const string fileName, vector<Material*> &materialStorage) {
    
    /* Opening file */
    string fullFileName = directory + fileName;
    ifstream file;   
    file.open(fullFileName.c_str(), ios::in | ios::binary);
    
    string buffer;
    string materialName = "";
    float ambient[] = {0.2, 0.2, 0.2, 1.0};
    float diffuse[] = {0.8, 0.8, 0.8, 1.0};
    float specular[] = {0, 0, 0, 1};
    float emition[] = {0, 0, 0, 1};
    float shininess = 0;
    
    while (getline(file, buffer, ' ')) {
        if (buffer == "newmtl") {
            getline(file, buffer);
            materialName = buffer;
        } else if (buffer == "Ns") {
            file >> shininess;
            getline(file, buffer);
        } else if (buffer == "Ka") {
            file >> ambient[0] >> ambient[1] >> ambient[2];
            RANGE(ambient[0], 0, 1);
            RANGE(ambient[1], 0, 1);
            RANGE(ambient[2], 0, 1);
            getline(file, buffer);
        } else if (buffer == "Kd") {
            file >> diffuse[0] >> diffuse[1] >> diffuse[2];
            RANGE(diffuse[0], 0, 1);
            RANGE(diffuse[1], 0, 1);
            RANGE(diffuse[2], 0, 1);
            getline(file, buffer);
        } else if (buffer == "Ks") {
            file >> specular[0] >> specular[1] >> specular[2];
            RANGE(specular[0], 0, 1);
            RANGE(specular[1], 0, 1);
            RANGE(specular[2], 0, 1);
            getline(file, buffer);
        } else if (buffer == "d" || buffer == "Tr") {
            file >> ambient[3];
            RANGE(ambient[3], 0, 1);
            diffuse[3] = specular[3] = emition[3] = ambient[3];
            getline(file, buffer);
        } else {
            getline(file, buffer);
        }
    }
    file.close();
    
    Material *material = new Material(materialName, ambient, diffuse, specular, emition, shininess);
    materialStorage.push_back(material);
}
Material *Objects3D::getMaterialByName(const vector<Material*> &materialStorage, const string &name) {
    for (unsigned i = 0; i < materialStorage.size(); i++) {
        if (materialStorage[i]->name() == name) {
            return materialStorage[i];
        } 
    }
    return NULL;
}