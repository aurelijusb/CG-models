3D objects
==========

Compilation
-----------

Libraries
 * OpenGl
 * Free GLUT

To compile program::

    gcc src/main.c -lGL -lGLU -lglut -lm -o U3Aurelijus
    g++ grafika2.cpp -lGL -lGLU -lglut -lm -o U3T


User interface
--------------

To view object, drag with mouse.
[1] to turn on/off axis
[2] to turn on/off game mode
[Q][W], [A][S], [Z][X] to rotate around red, gree and blue axes.

  
References
----------

 * http://uosis.mif.vu.lt/~rimask/destymas/kompiuterine-grafika-ps/3-uzd/
 * https://svn.blender.org/svnroot/bf-extensions/trunk/py/scripts/addons/add_mesh_solid.py
 * http://www.rwgrayprojects.com/rbfnotes/polyhed/PolyhedraData/RhombicTriaconta/RhombicTriaconta.pdf
 * http://scv.bu.edu/documentation/software-help/graphics-programming/opengl_examples/materials/