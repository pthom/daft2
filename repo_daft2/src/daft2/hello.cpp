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

    //glfwSetErrorCallback(consoleErrorHandler);

    std::cout << "About to initialize GLFW" << std::endl;
    glfwInit();
    std::cout << "GLFW initialized" << std::endl;

    // print the version on the console
    printf("%s\n", glfwGetVersionString());
}
