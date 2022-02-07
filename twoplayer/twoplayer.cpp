
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

#define GLFW_INCLUDE_NONE

#include "../glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <string>

#include "board_gui.h"

int initGUI(GLFWwindow *(&window), const unsigned int width, const unsigned int height);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void window_size_callback(GLFWwindow *window, int width, int height);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// window settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// entry point
// -------------------------------------------------------------------------
int main(void)
{
    GLFWwindow *window;
    if (!initGUI(window, SCR_WIDTH, SCR_HEIGHT))
        return -1;

    BoardGUI board_gui(SCR_WIDTH, SCR_HEIGHT);
    glfwSetWindowUserPointer(window, &board_gui);

    // render loop
    // -----------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        board_gui.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clearing all resources allocated to GLFW
    glfwTerminate();
    return 0;
}

// callback for mouse input
// -------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // pass mouse pos to board_gui
        BoardGUI *board_gui = (BoardGUI *)glfwGetWindowUserPointer(window);
        board_gui->setMousePress(xpos, ypos);
    }
}

// callback for framebuffer resize (by OS or user)
// -------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // setting viewport maps width and height to correct screen coordinates
    glViewport(0, 0, width, height);
}

// callback for window resize (by OS or user)
// -------------------------------------------------------------------------
void window_size_callback(GLFWwindow *window, int width, int height)
{
    // pass width and height to board_gui
    BoardGUI *board_gui = (BoardGUI *)glfwGetWindowUserPointer(window);
    board_gui->setWinDim(width, height);
}

// callback for key input
// -------------------------------------------------------------------------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // close window on spacebar
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // redo move on backspace
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
        // tell gui to cancel last move
        BoardGUI *board_gui = (BoardGUI *)glfwGetWindowUserPointer(window);
        board_gui->setCancelLastMove();
    }
}

// Initiate the GUI, including the GLFW library, GLAD library
// and the window. Returns 1 if successful and 0 otherwise.
// -------------------------------------------------------------------------
int initGUI(GLFWwindow *(&window), const unsigned int width, const unsigned int height)
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
    window = glfwCreateWindow(width, height, "test", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    // set callback functions
    // ---------------------------------------------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

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
    std::cout << "\nRenderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << "\n"
              << std::endl;
    return 1;
}
