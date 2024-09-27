#include <iostream>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <emscripten/version.h>
#include <emscripten.h>


void hello()
{
    std::cout << "Hello, World!" << std::endl;

    // print the version on the console
    printf("%s\n", glfwGetVersionString());
    printf("emscripten: v%d.%d.%d\n", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__);

    printf("Attempting to initialize GLFW\n");
    if(!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        return;
    }

    // auto window = glfwCreateWindow(320, 200, "example_minimal | emscripten-glfw", nullptr, nullptr);
    // glfwDestroyWindow(window);

    // glfwTerminate();    
}
