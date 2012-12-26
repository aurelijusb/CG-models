 //*******************************************************************
   // Function: CalculateVectorNormal
   // 
   // Purpose: Given three points of a 3D plane, this function calculates
   //          the normal vector of that plane.
   // 
   // Parameters:
   //     fVert1[]   == array for 1st point (3 elements are x, y, and z).
   //     fVert2[]   == array for 2nd point (3 elements are x, y, and z).
   //     fVert3[]   == array for 3rd point (3 elements are x, y, and z).
   // 
   // Returns:
   //     fNormalX   == X vector for the normal vector
   //     fNormalY   == Y vector for the normal vector
   //     fNormalZ   == Z vector for the normal vector
   // 
   // Comments:
   // 
   // History:  Date       Author        Reason
   //           3/22/95     GGB           Created
   //**********************************************************************

#include "normals.h"

   GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[],

                             GLfloat fVert3[], GLfloat *fNormalX,
                             GLfloat *fNormalY, GLfloat *fNormalZ)
    {
    GLfloat Qx, Qy, Qz, Px, Py, Pz;

   Qx = fVert2[0]-fVert1[0];
   Qy = fVert2[1]-fVert1[1];
   Qz = fVert2[2]-fVert1[2];
   Px = fVert3[0]-fVert1[0];
   Py = fVert3[1]-fVert1[1];
   Pz = fVert3[2]-fVert1[2];

   *fNormalX = Py*Qz - Pz*Qy;
   *fNormalY = Pz*Qx - Px*Qz;
   *fNormalZ = Px*Qy - Py*Qx;

   }
