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

// = = = = = = = = = = text (gl-text.c) = = = = = = = = = = 

// maximum number of text objects allowed to exist at once. this forms a static pool of this many objects
#define MAX_TEXT_OBJECTS 100

// an enum representing all of the different fonts, used for mapping purposes. def is "default"
typedef enum FontType {
    def = 1
} FontType;

// struct to store information about a font, subject to expansion
typedef struct Font {
    GLuint fontTexture;
    FontType id;
} Font;

// struct to store information about a text object on the screen, subject to major expansion
typedef struct TextObject {
    // normalized between -1 and 1
    float x;
    float y;
    float maxX;
    // gl stuff
    GLuint vao; // TODO use a global vao? this would be private to gl-text.c
    GLuint vbo;
    // other data
    FontType font;
    float fontSize;
} TextObject;

// TODO hide this in gl-text.c ??? probably
extern TextObject textObjects[MAX_TEXT_OBJECTS];

// inits the text system and shaders, if any step fails, this will return 0, else (succeded) it will return 1
int initTextSystem(void);

// TODO text layout management, does it happen once or many times?
// Does the Line and Word struct used for that get globalized?
// figure this out

// = = = = = = = = = = end text (gl-text.c) = = = = = = = = = = 

#endif
