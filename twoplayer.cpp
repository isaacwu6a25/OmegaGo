
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <math.h>

#include "engine2.h"
#include "shader.h"
// #include "line.h"
#include "board_gui.h"

int initGUI(const unsigned int width, const unsigned int height,
            const char *name, GLFWwindow *(&window));
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void renderBoard(void);

// window settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// entry point
// -------------------------------------------------------------------------
int main(void)
{
    int width, height;
    GLFWwindow *window;
    if (!initGUI(SCR_WIDTH, SCR_HEIGHT, "test", window))
        return -1;

    // Line line(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
    BoardGUI board_gui;

    // render loop
    // -----------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        glfwGetWindowSize(window, &width, &height);
        board_gui.draw(width, height);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clearing all resources allocated to GLFW
    glfwTerminate();
    return 0;
}

// process inputs
// -------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // query GLFW for key presses
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Initiate the GUI, including the GLFW library, GLAD library
// and the window. Returns 1 if successful and 0 otherwise.
// -------------------------------------------------------------------------
int initGUI(const unsigned int width, const unsigned int height,
            const char *name, GLFWwindow *(&window))
{
    // Initiate and configure GLFW library: Handles window creation
    // and destruction. Abstracts away OS specific operations.
    // ---------------------------------------------------------------
    if (!glfwInit())
    {
        std::cerr << "ERROR: Could not start GLFW3" << std::endl;
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // ---------------------------------------------------------------
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initiate GLAD library: Retrieves functions from the
    // OpenGL library for use directly without declaration.
    // ---------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "ERROR: Failed to initialize GLAD" << std::endl;
        return 0;
    }

    // report renderer and version
    // ---------------------------------------------------------------
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;
    return 1;
}

//  callback function is called whenever window is resized (by OS or user)
// -------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Processed coordinates in OpenGL are in (-1, 1), setting
    // viewport maps these values to correct screen coordinates.
    // ---------------------------------
    glViewport(0, 0, width, height);
}
