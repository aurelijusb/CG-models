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

