#include <iostream>
#include <GLFW/glfw3.h>

//! Display error message in the Console
void consoleErrorHandler(int iErrorCode, char const *iErrorMessage)
{
  printf("glfwError: %d | %s\n", iErrorCode, iErrorMessage);
}

void hello()
{
    glfwSetErrorCallback(consoleErrorHandler);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Set GLFW window hints for WebGL2
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL ES 3.0 corresponds to WebGL2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Optional, for forward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Initially hidden

    // Optional: Enable additional hints
    glfwWindowHint(GLFW_SAMPLES, 4); // For anti-aliasing (antialiasing was false earlier)
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    
    GLFWwindow* window = glfwCreateWindow(600, 400, "Test Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        //const char* description;
        //int code = glfwGetError(&description);
        //std::cerr << "GLFW Error (" << code << "): " << (description ? description : "No description") << std::endl;
        glfwTerminate();
        return;
    }

    std::cout << "Window created successfully!" << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();
  }
