3D objects
==========

Functionlality
--------------

OpengGl+Glut example which includes:
 * Model rotation (based on quaternions)
 * Animation (simple face move game)
 * Lighting, materials, smooth shading
 * Rotating objects along line (cylinders on edges)
 * Loading .obj files


Compilation
-----------

Libraries
 * OpenGl
 * Free GLUT

To compile program::

    gcc src/* -lGL -lGLU -lglut -lm -o U4Aurelijus


User interface
--------------

To view object, drag with mouse. Keyboard controllers
[1] to turn on/off axis
[2] to turn on/off game mode
[3] to turn on/off lightning
[4] to turn on/off skeleton (cilinders on edges, and spheres on vertexes)
[5] to turn on/off materials
[6] to turn on/off smooth shading
[7] to turn on/off loading external (.obj .mtl) object
[Q][W], [A][S], [Z][X] to rotate around red, gree and blue axes.

  
References
----------

 * http://uosis.mif.vu.lt/~rimask/destymas/kompiuterine-grafika-ps/3-uzd/
 * https://svn.blender.org/svnroot/bf-extensions/trunk/py/scripts/addons/add_mesh_solid.py
 * http://www.rwgrayprojects.com/rbfnotes/polyhed/PolyhedraData/RhombicTriaconta/RhombicTriaconta.pdf
 * http://scv.bu.edu/documentation/software-help/graphics-programming/opengl_examples/materials/
 * http://support.microsoft.com/kb/131130
 * http://www.vbforums.com/showthread.php?584390-Quaternion-from-two-3D-Position-Vectors
 * http://www.opengl.org/discussion_boards/showthread.php/152034-Angle-between-two-vectors