#include "gl.h"

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
