#include <iostream>
#include <GLFW/glfw3.h>

//! Display error message in the Console
void consoleErrorHandler(int iErrorCode, char const *iErrorMessage)
{
  printf("glfwError: %d | %s\n", iErrorCode, iErrorMessage);
}

void hello()
{
    std::cout << "Hello, World!" << std::endl;

    std::cout << "About to set error callback" << std::endl;
    glfwSetErrorCallback(consoleErrorHandler);
    std::cout << "Error callback set" << std::endl;

    std::cout << "About to initialize GLFW" << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    std::cout << "GLFW initialized" << std::endl;

    // Set GLFW window hints for WebGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // WebGL 2.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Initially hidden

    // print the version on the console
    printf("%s\n", glfwGetVersionString());

    std::cout << "About to create window" << std::endl;
    GLFWwindow* window = glfwCreateWindow(600, 400, "Hello GLFW", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;

        glfwTerminate();
        return;
    } 
    std::cout << "Window created" << std::endl;

    std::cout << "About to make context current" << std::endl;
    glfwMakeContextCurrent(window);
    std::cout << "Context made current" << std::endl;

    // Optional: Show the window if needed
    //glfwShowWindow(window);

    /*
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    */

    std::cout << "About to destroy window" << std::endl;
    glfwDestroyWindow(window);
    std::cout << "Window destroyed" << std::endl;
    std::cout << "About to terminate GLFW" << std::endl;
    glfwTerminate();    
    std::cout << "GLFW terminated" << std::endl;
}
