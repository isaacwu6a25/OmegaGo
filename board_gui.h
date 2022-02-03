// line class

#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>
#include <string>

#include "shader.h"

// shader source codes
// ---------------------------------------------------------
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

// Line class:
// handles buffer & shader construction, and drawing.
// -----------------------------------------------------------------
class BoardGUI
{
    unsigned int VAO, VBO;
    std::array<float, 152> vertices;
    std::array<int, 76> indices;
    glm::vec3 lineColor, backColor;

    Shader shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);

public:
    // constructor
    // -------------------------------------------------------------

    BoardGUI()
    {
        backColor = glm::vec3(0.6f, 0.5f, 0.4f);
        lineColor = glm::vec3(0.0f, 0.0f, 0.0f);

        // vertex data
        // ------------------------------------
        const float spacing = 2.0 / 18;
        for (int n = 0; n < 19; n++)
        {
            vertices[4 * n] = -1.0f;
            vertices[4 * n + 2] = 1.0f;
            vertices[4 * n + 1] = vertices[4 * n + 3] = -1.0f + spacing * n;
            vertices[76 + 4 * n + 1] = -1.0f;
            vertices[76 + 4 * n + 3] = 1.0f;
            vertices[76 + 4 * n] = vertices[76 + 4 * n + 2] = -1.0f + spacing * n;
        }

        // index data
        // ------------------------------------
        for (int n = 0; n < 76; n++)
            indices[n] = n;

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

    // set colors
    // -------------------------------------------------------------
    void setLineColor(glm::vec3 color)
    {
        lineColor = color;
    }
    void setBackColor(glm::vec3 color)
    {
        backColor = color;
    }

    // handles drawing
    // -------------------------------------------------------------
    void draw(const int width, const int height)
    {
        // clearing before each render
        glClearColor(backColor.x, backColor.y, backColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.9 * height / width, 0.9, 1.0));

        shaderProgram.use();
        shaderProgram.setVec3("color", lineColor);
        shaderProgram.setMat4("transform", trans);

        glBindVertexArray(VAO);
        glEnable(GL_LINE_SMOOTH);
        glDrawArrays(GL_LINES, 0, 152);
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