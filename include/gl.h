#ifndef FIGHT_GL_H
#define FIGHT_GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct ShaderProgram {
    GLuint program;
    GLuint fragment;
    GLuint vertex;
} ShaderProgram;

const GLFWvidmode * setWindowedFullscreenHints(GLFWmonitor* mon);

#endif
