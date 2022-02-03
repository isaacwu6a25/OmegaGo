// line class

#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <array>
#include <string>

#include "shader.h"

// shader source codes
// ---------------------------------------------------------
const std::string vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
class Line
{
    unsigned int VAO, VBO;
    std::array<float, 6> vertices;
    glm::vec3 lineColor;

    Shader shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);

public:
    // constructor
    // -------------------------------------------------------------

    Line(glm::vec3 start, glm::vec3 end)
    {
        lineColor = glm::vec3(1.0f, 1.0f, 1.0f);
        // vertex data
        // ------------------------------------
        vertices = {start.x, start.y, start.z,
                    end.x, end.y, end.z};

        // buffers
        // ------------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // set line color
    // -------------------------------------------------------------
    int setColor(glm::vec3 color)
    {
        lineColor = color;
        return 1;
    }

    // handles drawing
    // -------------------------------------------------------------
    int draw()
    {
        shaderProgram.use();
        shaderProgram.setVec3("color", lineColor);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        return 1;
    }

    // handles destruction
    // -------------------------------------------------------------
    ~Line()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif