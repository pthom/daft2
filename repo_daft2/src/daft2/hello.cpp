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

    // set a callback for errors otherwise if there is a problem, we won't know
    glfwSetErrorCallback(consoleErrorHandler);

    // print the version on the console
    printf("%s\n", glfwGetVersionString());
}
