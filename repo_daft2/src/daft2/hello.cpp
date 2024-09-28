#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
// include OpenGL
#include <GLES3/gl3.h>


// Vertex Shader Source (OpenGL ES 3.0)
const char* vertexShaderSource = R"(#version 300 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 rotation;

out vec3 ourColor;

void main()
{
    gl_Position = rotation * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

// Fragment Shader Source (OpenGL ES 3.0)
const char* fragmentShaderSource = R"(#version 300 es
precision mediump float;

in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)";

// Global variables for shader program and VAO
GLuint shaderProgram;
GLuint VAO;

// Function to compile a shader and check for errors
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // Get and print the error log
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Initialize GLFW, OpenGL, shaders, and buffers
bool initialize() {
    // Set error callback
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set GLFW window hints for OpenGL ES 3.0 (WebGL2)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL ES 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Optional
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // Make window visible

    // Additional hints
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(600, 400, "Rotating Square", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Enable V-Sync
    glfwSwapInterval(1);

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link shaders into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking status
    GLint successLink;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successLink);
    if (!successLink) {
        // Get and print the error log
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // Shaders are linked into the program; no longer needed separately
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define the square vertices with positions and colors
    float vertices[] = {
        // Positions         // Colors
        // First Triangle
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom Left (Red)
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom Right (Green)
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Top Right (Blue)
        // Second Triangle
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom Left (Red)
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Top Right (Blue)
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // Top Left (Yellow)
    };

    // Generate and bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO (optional)
    glBindVertexArray(0);

    // Enable depth testing (optional for 2D)
    glEnable(GL_DEPTH_TEST);

    // Store the window pointer for later use
    glfwSetWindowUserPointer(window, window);

    return true;
}

// Function to create a rotation matrix around the Z-axis
void createRotationMatrix(float angle, float* matrix) {
    float radians = angle * (3.14159265f / 180.0f);
    float cosA = cosf(radians);
    float sinA = sinf(radians);

    // Column-major order
    matrix[0] = cosA;  matrix[4] = -sinA; matrix[8]  = 0.0f; matrix[12] = 0.0f;
    matrix[1] = sinA;  matrix[5] = cosA;  matrix[9]  = 0.0f; matrix[13] = 0.0f;
    matrix[2] = 0.0f;  matrix[6] = 0.0f;  matrix[10] = 1.0f; matrix[14] = 0.0f;
    matrix[3] = 0.0f;  matrix[7] = 0.0f;  matrix[11] = 0.0f; matrix[15] = 1.0f;
}

// Render function: clears the screen, applies rotation, and draws the square
void render() {
    // Get the current time for rotation
    float timeValue = static_cast<float>(glfwGetTime());
    float angle = timeValue * 50.0f; // Rotate 50 degrees per second

    // Create rotation matrix
    float rotationMatrix[16];
    createRotationMatrix(angle, rotationMatrix);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Pass the rotation matrix to the shader
    GLint rotLoc = glGetUniformLocation(shaderProgram, "rotation");
    glUniformMatrix4fv(rotLoc, 1, GL_FALSE, rotationMatrix);

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Dark teal background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Draw the square (6 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind VAO (optional)
    glBindVertexArray(0);
}

// Cleanup function: deletes buffers and terminates GLFW
void terminateGLFW() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void hello()
{
  printf("Hello, World!\n");
}