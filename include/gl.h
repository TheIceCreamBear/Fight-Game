#ifndef FIGHT_GL_H
#define FIGHT_GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct ShaderProgram {
    GLuint program;
    GLuint fragment;
    GLuint vertex;
} ShaderProgram;
// TODO uniforms somehow...

const GLFWvidmode * setWindowedFullscreenHints(GLFWmonitor* mon);

// = = = = = = = = = = shaders = = = = = = = = = = 
// creates a new shader program from the given path to the vertex and fragment shader files. Returns NULL on failure
ShaderProgram * creteShaderProgram(const char* pathToVertex, const char* pathToFrag, void (*bindAttribs)(GLuint));

// cleans up the shader via opengl calls, then frees the given pointer
void cleanUpShaderProgram(ShaderProgram* shader);

// cleans up all shaders stored in spll. THIS IS DESTRUCTIVE and will DESTORY THE LIST!! 
void cleanUpAllShaders(void);

// starts the given shader
void startShader(ShaderProgram* shader);

// helper to stop the current shader
void stopShader(void);
// = = = = = = = = = = end shaders = = = = = = = = = = 

// = = = = = = = = = = vaos/vbos = = = = = = = = = = 
// creates a new opengl VAO
GLuint createVao(void);

// helper to unbind currently bound vao
void unbindVao(void);

// cleans up the vaos and vbos. THIS IS DESTRUCTIVE and will DESTORY THE LISTS!!
void cleanUpVaosVbos(void);

// puts the given data into a vbo
void dataToAttribList(int attribNum, int attribSize, float* data, int dataLength);
// = = = = = = = = = = end vaos/vbos = = = = = = = = = = 

#endif
