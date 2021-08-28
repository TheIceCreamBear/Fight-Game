#include "gl.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

const GLFWvidmode * setWindowedFullscreenHints(GLFWmonitor* mon) {
    const GLFWvidmode* vidMode = glfwGetVideoMode(mon);

    // set the window to use the current video mode
    glfwWindowHint(GLFW_RED_BITS, vidMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, vidMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, vidMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, vidMode->refreshRate);

    // dont let it be resizeable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return vidMode;
}

// = = = = = = = = = = shaders = = = = = = = = = = 
// spll, or shader program linked list, is a linked list wrapper for shaders making cleanup easier
struct spll {
    ShaderProgram* sp;
    struct spll* next;
};

// global (but private to gl.c) head for the spll
struct spll* spllHead = NULL;
// global (but private to gl.c) tail for the spll for O(1) insertion
struct spll* spllTail = NULL;

// adds the given shader to the spll for global cleanup later. returns 1 on success and 0 on failure
int addShaderToSpll(ShaderProgram* sp) {
    struct spll* splli = malloc(sizeof(struct spll));
    if (splli == NULL) {
        return 0;
    }

    // add shader to node
    splli->sp = sp;
    splli->next = NULL; // for saftey

    // empty list
    if (spllHead == NULL) {
        spllHead = splli;
        spllTail = spllHead;
        return 1;
    }

    // non empty list
    spllTail->next = splli;
    spllTail = spllTail->next;
    return 1;
}

// local function to gl.c used to load the shader from the file into memory and compile it. returns -1 on any failure.
GLuint loadShader(const char* path, GLenum type) {
    // TODO read file
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, FATAL "Could not open file %s\n", path);
        return -1;
    }

    // seek to end, ftell file size, check for errors
    if (fseek(file, 0, SEEK_END) == -1) { // TODO this may not be portable. This is a problem for future me, not current me
        fprintf(stderr, FATAL "Seek on file %s failed\n", path);
    }

    long llength = ftell(file);
    if (llength <= 0 || llength > 0xffffff00) {
        // if file size is 0 or smaller, or larger than a value that can be stored in an int
        fprintf(stderr, FATAL "Invalid file (%s) length: %ld\n", path, llength);
    }

    // cast length to avoid warnings
    int length = (int) llength;

    // rewind back to beginning so we can read the file
    rewind(file);

    // malloc shader source
    char* shaderSource = malloc(sizeof(char) * length);
    char in = '\0';
    int i = 0;

    // read file char by char
    while (in != EOF && i < length) {
        in = getc(file);
        shaderSource[i] = in;
        i++;
    }

    // close the file
    fclose(file);

    // TODO determine if ARB calls are needed, if we use any extensions

    // create const shader source ptr
    const char* sorucePtr = shaderSource;
    const char* const* passablePtr = &sorucePtr;

    // create shader, load shader source, and then free the source
    int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, passablePtr, &length);
    free(shaderSource);

    // compile the shader
    glCompileShader(shaderID);

    // check for compile error
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        // failed to compile
        GLint bufferLen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &bufferLen);
        if (bufferLen > 1) {
            // malloc string and pull from info log
            GLchar* buf = malloc(bufferLen);
            GLsizei slen;
            glGetShaderInfoLog(shaderID, bufferLen, &slen, buf);

            // print error
            fprintf(stderr, FATAL "Shader %s failed to compile:\n%s\n", path, buf);

            // free buffer
            free(buf);
        }

        // return failure
        return -1;
    }

    return shaderID;
}

// creates a new shader program from the given path to the vertex and fragment shader files. Returns NULL on failure
ShaderProgram * creteShaderProgram(const char* pathToVertex, const char* pathToFrag, void (*bindAttribs)(GLuint)) {
    ShaderProgram* sp = malloc(sizeof(ShaderProgram));
    if (sp == NULL) {
        return NULL;
    }
    
    // load the vertex shader
    GLuint vert = loadShader(pathToVertex, GL_VERTEX_SHADER);
    if (vert == -1) {
        free(sp);
        return NULL;
    }

    // load the fragment shader
    GLuint frag = loadShader(pathToFrag, GL_FRAGMENT_SHADER);
    if (frag == -1) {
        glDeleteShader(vert);
        free(sp);
        return NULL;
    }

    // create program and attach shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);

    // TODO compile and link
    // used to bind the 
    bindAttribs(program);

    // link program
    glLinkProgram(program);

    // check link error
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        // failed to compile
        GLint bufferLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufferLen);
        if (bufferLen > 1) {
            // malloc string and pull from info log
            GLchar* buf = malloc(bufferLen);
            GLsizei slen;
            glGetProgramInfoLog(program, bufferLen, &slen, buf);

            // print error
            fprintf(stderr, FATAL "Shader (%s, %s) failed to link:\n%s\n", pathToVertex, pathToFrag, buf);

            // free buffer
            free(buf);
        }

        // free created things
        cleanUpShaderProgram(sp);

        // return failure
        return NULL;
    }

    // validate program
    #ifdef FIGHT_VALIDATE_SHADER // disable shader validation here unless FIGHT_VALIDATE_SHADER is defined
    glValidateProgram(program);

    // check validation error
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        // failed to compile
        GLint bufferLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufferLen);
        if (bufferLen > 1) {
            // malloc string and pull from info log
            GLchar* buf = malloc(bufferLen);
            GLsizei slen;
            glGetProgramInfoLog(program, bufferLen, &slen, buf);

            // print error
            fprintf(stderr, FATAL "Shader (%s, %s) failed to validate:\n%s\n", pathToVertex, pathToFrag, buf);

            // free buffer
            free(buf);
        }

        // free created things
        cleanUpShaderProgram(sp);

        // return failure
        return NULL;
    }
    #endif // FIGHT_VALIDATE_SHADER

    sp->program = program;
    sp->vertex = vert;
    sp->fragment = frag;

    // if adding to spll failed, free the struct and return NULL
    if (!addShaderToSpll(sp)) {
        cleanUpShaderProgram(sp);
        return NULL;
    }

    // return generated shader program
    return sp;
}

// cleans up the shader via opengl calls, then frees the given pointer
void cleanUpShaderProgram(ShaderProgram* shader) {
    // detach the shaders
    glDetachShader(shader->program, shader->vertex);
    glDetachShader(shader->program, shader->fragment);

    // delete the shaders
    glDeleteShader(shader->vertex);
    glDeleteShader(shader->fragment);

    // delete the program
    glDeleteProgram(shader->program);

    // free the struct
    free(shader);
}

// cleans up all shaders stored in spll. THIS IS DESTRUCTIVE and will DESTORY THE LIST!! 
void cleanUpAllShaders(void) {
    struct spll* lostpointer = spllHead;
    while (spllHead != NULL) {
        // clean the shader
        cleanUpShaderProgram(spllHead->sp);

        // move pointer, free lostpointer, update lostpointer
        spllHead = spllHead->next;
        free(lostpointer);
        lostpointer = spllHead;
    }
}

// starts the given shader
void startShader(ShaderProgram* shader) {
    glUseProgram(shader->program);
}

// helper to stop the current shader
void stopShader(void) {
    glUseProgram(0);
}
// = = = = = = = = = = end shaders = = = = = = = = = = 

// = = = = = = = = = = vaos/vbos = = = = = = = = = = 
struct vaoll {
    GLuint vao;
    struct vaoll* next;
};
struct vboll {
    GLuint vbo;
    struct vboll* next;
};

struct vaoll* vaollHead = NULL;
struct vaoll* vaollTail = NULL;
struct vboll* vbollHead = NULL;
struct vboll* vbollTail = NULL;

// adds the given vao to the vaoll for global cleanup later. returns 1 on success and 0 on failure
int addVao(GLuint vao) {
    struct vaoll* vaolli = malloc(sizeof(struct vaoll));
    if (vaolli == NULL) {
        return 0;
    }

    // add shader to node
    vaolli->vao = vao;
    vaolli->next = NULL; // for saftey

    // empty list
    if (vaollHead == NULL) {
        vaollHead = vaolli;
        vaollTail = vaollHead;
        return 1;
    }

    // non empty list
    vaollTail->next = vaolli;
    vaollTail = vaollTail->next;
    return 1;
}

// adds the given vbo to the vboll for global cleanup later. returns 1 on success and 0 on failure
int addVbo(GLuint vbo) {
    struct vboll* vbolli = malloc(sizeof(struct vboll));
    if (vbolli == NULL) {
        return 0;
    }

    // add shader to node
    vbolli->vbo = vbo;
    vbolli->next = NULL; // for saftey

    // empty list
    if (vbollHead == NULL) {
        vbollHead = vbolli;
        vbollTail = vbollHead;
        return 1;
    }

    // non empty list
    vbollTail->next = vbolli;
    vbollTail = vbollTail->next;
    return 1;
}

// creates a new opengl VAO
GLuint createVao(void) {
    // generate vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    // add to list
    addVao(vao);
    // bind it case we are about to use it
    glBindVertexArray(vao);

    return vao;
}

// helper to unbind currently bound vao
void unbindVao(void) {
    glBindVertexArray(0);
}

// cleans up the vaos and vbos
void cleanUpVaosVbos(void) {
    struct vaoll* vaolostpointer = vaollHead;
    while (vaollHead != NULL) {
        // clean the shader
        glDeleteVertexArrays(1, &vaollHead->vao);

        // move pointer, free vaolostpointer, update vaolostpointer
        vaollHead = vaollHead->next;
        free(vaolostpointer);
        vaolostpointer = vaollHead;
    }

    struct vboll* vbolostpointer = vbollHead;
    while (vbollHead != NULL) {
        // clean the shader
        glDeleteBuffers(1, &vbollHead->vbo);

        // move pointer, free vbolostpointer, update vbolostpointer
        vbollHead = vbollHead->next;
        free(vbolostpointer);
        vbolostpointer = vbollHead;
    }
}

// puts the given data into a vbo
void dataToAttribList(int attribNum, int attribSize, float* data, int dataLength) {
    // generate vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // add to list
    addVbo(vbo);

    // bind
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // put data in buffer
    glBufferData(GL_ARRAY_BUFFER, dataLength * sizeof(float), data, GL_STATIC_DRAW);

    // mark this vbo as part of the bound vao
    glVertexAttribPointer(attribNum, attribSize, GL_FLOAT, GL_FALSE, 0, 0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// = = = = = = = = = = end vaos/vbos = = = = = = = = = = 