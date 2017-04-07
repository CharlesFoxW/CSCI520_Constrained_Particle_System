//
// Created by Clarence Wang on 4/5/17.
//

#ifndef CSCI520PROJ3_OPENGL_HEADERS_H_H
#define CSCI520PROJ3_OPENGL_HEADERS_H_H
#if defined(WIN32) || defined(linux)
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#endif

#endif //CSCI520PROJ3_OPENGL_HEADERS_H_H
