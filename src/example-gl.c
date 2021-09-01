#include <stdio.h>

#include "example-gl.h"

void errorCallback(int errorCode, const char* msg) {
    fprintf(stderr, "ERROR: GLFW encountered an error: [%x]: %s\n", errorCode, msg);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    // depending on the number of windows, a window verificaiton would occur here

    // this will close the window when the user hits escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// prototype so it can be called in the GLFW example
void simpleGLExample(GLFWwindow* window);

void simpleGLFWExample(void) {
    // error call back to log any errors that occur
    glfwSetErrorCallback(errorCallback);

    // init GLFW cause we need it bruv
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not initialize GLFW window system.\n");
        return;
    }

    // this sets up our version to be 4.1 (not what ever the GC wants)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // set the hints for the windowed full screen
    GLFWmonitor* primaryMon = glfwGetPrimaryMonitor();
    const GLFWvidmode* primaryMonVid = setWindowedFullscreenHints(primaryMon);

    // create the window
    GLFWwindow* window = glfwCreateWindow(primaryMonVid->width, primaryMonVid->height, "The title is now", primaryMon, NULL);

    // TODO set input call backs
    glfwSetKeyCallback(window, keyCallback);

    // tells GLFW to use this window for open gl calls
    glfwMakeContextCurrent(window);

    // enables v sync, optional, helps not kill GPUs tho (looking at your New World)
    glfwSwapInterval(1);

    // if we wanted to, the opengl stuff would go here
    simpleGLExample(window);

    // clean up our mess
    cleanUpVaosVbos();
    cleanUpAllShaders();

    // stop GLFW stuff
    glfwTerminate();
}

void bindExampleAttribs(GLuint program) {
    glBindAttribLocation(program, 0, "pos");
}

void simpleGLExample(GLFWwindow* window) {
    // init glew
    glewExperimental = GL_TRUE;
    glewInit();

    // print some version info
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version : %s\n", glGetString(GL_VERSION));

    // time related vars
    double lastFrameTime = glfwGetTime();
    double dDelta = 0.0f;
    float delta = 0.0f;
    float current = 0.0f;

    // TODO init gl objects
    ShaderProgram* exampleShader = creteShaderProgram("res/shdr/example.vert", "res/shdr/example.frag", bindExampleAttribs);
    if (exampleShader == NULL) {
        return;
    }
    GLint colorLoc = glGetUniformLocation(exampleShader->program, "color");

    // = = = = = = = = = = initialize rectangle = = = = = = = = = = 
    float verticies[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    int indices[] = {
        0, 1, 3,
        3, 1, 2
    };
    int vertexCount = 6; // lengeth of indicies

    GLuint rectVao = createVao();
    dataToAttribList(0, 3, verticies, 12);
    bindIndiciesBuffer(indices, 6);
    unbindVao();
    // = = = = = = = = = = end initialize rectangle = = = = = = = = = = 

    float r = 1.0f;
    float g = 0.0f;
    float b = 0.0f;
    float rgbspeed = 0.75f;
    int rgbstate = 1;

    // while the window hasnt been told to close, update the window
    while (!glfwWindowShouldClose(window)) {
        // = = = = = = = = = = update here = = = = = = = = = =
        // sets the clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // makes clear color rainbow
        float drgb = rgbspeed * delta;
        switch (rgbstate) {
            case 0:
                b -= drgb;
                r += drgb;
                if (b <= 0.0f) {
                    b = 0.0f;
                }
                if (r >= 1.0f) {
                    r = 1.0f;
                }
                if (b <= 0.0f && r >= 1.0f) {
                    rgbstate = 1;
                }
                break;
            case 1:
                r -= drgb;
                g += drgb;
                if (r <= 0.0f) {
                    r = 0.0f;
                }
                if (g >= 1.0f) {
                    g = 1.0f;
                }
                if (r <= 0.0f && g >= 1.0f) {
                    rgbstate = 2;
                }
                break;
            case 2:
                g -= drgb;
                b += drgb;
                if (g <= 0.0f) {
                    g = 0.0f;
                }
                if (b >= 1.0f) {
                    b = 1.0f;
                }
                if (g <= 0.0f && b >= 1.0f) {
                    rgbstate = 0;
                }
                break;
        }

        // = = = = = = = = = = end update = = = = = = = = = =

        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // = = = = = = = = = = render = = = = = = = = = = = 
        startShader(exampleShader);
        glUniform3f(colorLoc, r, g, b);
        
        // draw rectangle
        glBindVertexArray(rectVao);
        glEnableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, indices);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        
        stopShader();
        // = = = = = = = = = = end render = = = = = = = = = = = 

        // read events that came in while we were rendering
        glfwPollEvents();

        // this is what puts our image out to the display
        glfwSwapBuffers(window);

        // calculate how long it took to render
        double curFrame = glfwGetTime();
        dDelta = curFrame - lastFrameTime;
        lastFrameTime = curFrame;
        delta = (float) dDelta;
    }
}
