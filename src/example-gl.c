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

    // stop GLFW stuff
    glfwTerminate();
}

void simpleGLExample(GLFWwindow* window) {
    // init glew
    glewExperimental = GL_TRUE;
    glewInit();

    // print some version info
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version : %s\n", glGetString(GL_VERSION));

    // TODO init gl objects

    // while the window hasnt been told to close, update the window
    while (!glfwWindowShouldClose(window)) {
        // update here

        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render here

        // read events that came in while we were rendering
        glfwPollEvents();

        // this is what puts our image out to the display
        glfwSwapBuffers(window);
    }
}