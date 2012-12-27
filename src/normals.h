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

#ifndef NORMALS_H
#define	NORMALS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef	GLvoid
#include <GL/gl.h>
#endif
    
    
GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[],
                             GLfloat fVert3[], GLfloat *fNormalX,
                             GLfloat *fNormalY, GLfloat *fNormalZ);

#ifdef	__cplusplus
}
#endif

#endif	/* NORMALS_H */

