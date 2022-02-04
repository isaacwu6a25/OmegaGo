// line class

#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>
#include <string>
#include <cmath>

#include "shader.h"
#include "engine2.h"

#define VERT_NUM BOARD_SIZE * 4

// shader source codes
// -----------------------------------------------------------------
const std::string vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 0.0f, 1.0f);\n"
    "}\0";
const std::string fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(color, 1.0f);\n"
    "}\0";

// GUI: handles buffer & shader construction, and drawing.
// -----------------------------------------------------------------
class BoardGUI
{
    // dimensions
    int win_width, win_height;
    glm::vec2 mousePos;
    glm::mat4 trans;
    const float spacing = 2.0 / (BOARD_SIZE - 1);

    // board lines
    unsigned int VAO, VBO;
    std::array<float, VERT_NUM * 2> vertices;
    glm::vec3 lineColor, backColor;

    // universal shader
    Shader shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);

    // game engine
    GameEngine go_game{0};
    bool UPDATE_GUI = false;

public:
    // constructor
    // -------------------------------------------------------------

    BoardGUI(int width, int height)
    {
        setWinDim(width, height);

        backColor = glm::vec3(0.6f, 0.5f, 0.4f);
        lineColor = glm::vec3(0.0f, 0.0f, 0.0f);

        // vertex data
        // ------------------------------------
        for (int n = 0; n < BOARD_SIZE; n++)
        {
            vertices[4 * n] = -1.0f;
            vertices[4 * n + 2] = 1.0f;
            vertices[4 * n + 1] = vertices[4 * n + 3] = -1.0f + spacing * n;
            vertices[VERT_NUM + 4 * n + 1] = -1.0f;
            vertices[VERT_NUM + 4 * n + 3] = 1.0f;
            vertices[VERT_NUM + 4 * n] = vertices[VERT_NUM + 4 * n + 2] = -1.0f + spacing * n;
        }

        // buffers
        // ------------------------------------
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // set variables
    // -------------------------------------------------------------
    void setLineColor(glm::vec3 color)
    {
        lineColor = color;
    }
    void setBackColor(glm::vec3 color)
    {
        backColor = color;
    }
    void setWinDim(int width, int height)
    {
        win_width = width;
        win_height = height;

        // transformation
        trans = glm::scale(glm::mat4(1.0f),
                           glm::vec3(0.9 * win_height / win_width, 0.9, 1.0));
    }
    void setMousePress(double x, double y)
    {
        // calculate intended move
        double fac = 1 / 0.9 / win_height;
        mousePos = glm::scale(glm::mat4(1.0f), glm::vec3(fac, fac, 1.0)) *
                   glm::vec4(x - win_width / 2, y - win_height / 2, 0.0f, 1.0f);
        mousePos += glm::vec2(0.5, 0.5);
        mousePos *= (BOARD_SIZE - 1);
        move_t move = (int)std::round(mousePos.x) + BOARD_SIZE * (int)std::round(mousePos.y);

        // attempt move and set update flag if move is valid
        qstate_ptr new_qstate = go_game.get_new_qstate(move, go_game.get_curr_qstate());
        if (new_qstate == nullptr || !go_game.push_new_qstate(new_qstate))
        {
            std::cout << "Invalid move!" << std::endl;
            return;
        }
        UPDATE_GUI = true;
        go_game.print_qstate(*new_qstate);
    }

    // handles drawing
    // -------------------------------------------------------------
    void render()
    {
        // clearing before each render
        glClearColor(backColor.x, backColor.y, backColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setVec3("color", lineColor);
        shaderProgram.setMat4("transform", trans);

        glBindVertexArray(VAO);
        glEnable(GL_LINE_SMOOTH);
        glDrawArrays(GL_LINES, 0, VERT_NUM * 2);
    }

    // handles destruction
    // -------------------------------------------------------------
    ~BoardGUI()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif