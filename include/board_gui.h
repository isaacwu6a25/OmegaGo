// line class

#ifndef BOARD_GUI_H_INCLUDED
#define BOARD_GUI_H_INCLUDED

#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <array>
#include <string>
#include <cmath>

#include "shader.h"
#include "engine.h"

#define LINE_VERT_NUM BOARD_SIZE * 4
#define CIRC_NGON 32
#define CIRC_VERT_NUM (CIRC_NGON + 1)
#define CIRC_RAD 0.9 / (BOARD_SIZE - 1)

// shader source codes
// -----------------------------------------------------------------
const std::string vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform vec2 stranslate;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   mat4 translate1 = mat4(1.0, 0.0, 0.0, 0.0,\n"
    "				           0.0, 1.0, 0.0, 0.0,\n"
    "				           0.0, 0.0, 1.0, 0.0,\n"
    "				           stranslate.x, stranslate.y, 0.0, 1.0);\n"
    "   gl_Position = transform * translate1 * vec4(aPos, 0.0f, 1.0f);\n"
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
    glm::mat4 board_scaling;
    const float spacing = 2.0 / (BOARD_SIZE - 1);

    // board lines
    unsigned int lVAO, lVBO;
    std::array<float, LINE_VERT_NUM * 2> line_verts{};
    glm::vec3 lineColor, backColor;

    // stones
    unsigned int cVAO, cVBO, cEBO;
    std::array<float, CIRC_VERT_NUM * 2> circ_verts{};
    std::array<int, CIRC_NGON * 3> circ_indices{};
    glm::vec3 blackColor, whiteColor;

    // universal shader
    Shader shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);

    // game engine
    GameEngine go_game{0};
    bool UPDATE_GUI = true; // pull once at game start
    board_t curr_black, curr_white;

    // updates cached board state
    // -------------------------------------------------------------
    void update_board()
    {
        curr_black = go_game.get_curr_qstate()[0];
        curr_white = go_game.get_curr_qstate()[READ_BACK + 1];
        UPDATE_GUI = false;
    }

    // generate circle vertices and indices
    // -------------------------------------------------------------
    void gen_circ_arrays()
    {
        double dtheta = 2 * M_PI / CIRC_NGON;
        for (int i = 0; i < CIRC_NGON; i++)
        {
            // vertex positions
            circ_verts[(i + 1) * 2] = CIRC_RAD * cos(i * dtheta);
            circ_verts[(i + 1) * 2 + 1] = CIRC_RAD * sin(i * dtheta);

            // indices
            circ_indices[3 * i + 1] = i + 1;
            (i + 1 == CIRC_NGON) ? circ_indices[3 * i] = 1 : circ_indices[3 * i + 2] = i + 2;
        }
    }

    // generate line vertices
    // -------------------------------------------------------------
    void gen_line_arrays()
    {
        for (int n = 0; n < BOARD_SIZE; n++)
        {
            line_verts[4 * n] = -1.0f;
            line_verts[4 * n + 2] = 1.0f;
            line_verts[4 * n + 1] = line_verts[4 * n + 3] = -1.0f + spacing * n;
            line_verts[LINE_VERT_NUM + 4 * n + 1] = -1.0f;
            line_verts[LINE_VERT_NUM + 4 * n + 3] = 1.0f;
            line_verts[LINE_VERT_NUM + 4 * n] = line_verts[LINE_VERT_NUM + 4 * n + 2] = -1.0f + spacing * n;
        }
    }

    // generate buffers
    // -------------------------------------------------------------
    void gen_buffers()
    {
        // lines
        glGenVertexArrays(1, &lVAO);
        glBindVertexArray(lVAO);

        glGenBuffers(1, &lVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(line_verts), line_verts.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // circles (stones and star points)
        glGenVertexArrays(1, &cVAO);
        glBindVertexArray(cVAO);

        glGenBuffers(1, &cVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circ_verts), circ_verts.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &cEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circ_indices), circ_indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // draw lines
    // -------------------------------------------------------------
    void draw_lines()
    {
        shaderProgram.setVec3("color", lineColor);
        shaderProgram.setVec2("stranslate", glm::vec2(0.0f, 0.0f));
        shaderProgram.setMat4("transform", board_scaling);

        glBindVertexArray(lVAO);
        glEnable(GL_LINE_SMOOTH);
        glDrawArrays(GL_LINES, 0, LINE_VERT_NUM * 2);
    }

    // draw stones
    // -------------------------------------------------------------
    void draw_stones()
    {
        glBindVertexArray(cVAO);
        glEnable(GL_LINE_SMOOTH);
        shaderProgram.setMat4("transform", board_scaling);

        for (int n = 0; n != TOTAL_SIZE; n++)
        {
            // black
            if (curr_black[n] == NEMPTY)
            {
                int move_x = n % BOARD_SIZE - 9;
                int move_y = n / BOARD_SIZE - 9;
                glm::vec2 move_vec((double)move_x / 9, -(double)move_y / 9);
                shaderProgram.setVec3("color", blackColor);
                shaderProgram.setVec2("stranslate", move_vec);
                glDrawElements(GL_TRIANGLES, 3 * CIRC_NGON, GL_UNSIGNED_INT, (void *)0);
            }
            // white
            else if (curr_white[n] == NEMPTY)
            {
                int move_x = n % BOARD_SIZE - 9;
                int move_y = n / BOARD_SIZE - 9;
                glm::vec2 move_vec((double)move_x / 9, -(double)move_y / 9);
                shaderProgram.setVec3("color", whiteColor);
                shaderProgram.setVec2("stranslate", move_vec);
                glDrawElements(GL_TRIANGLES, 3 * CIRC_NGON, GL_UNSIGNED_INT, (void *)0);
            }
        }
    }

public:
    // constructor
    // -------------------------------------------------------------
    BoardGUI(int width, int height)
    {
        setWinDim(width, height);

        // colors
        backColor = glm::vec3(0.7f, 0.6f, 0.5f);
        lineColor = glm::vec3(0.0f, 0.0f, 0.0f);
        blackColor = glm::vec3(0.1f, 0.1f, 0.1f);
        whiteColor = glm::vec3(0.9f, 0.9f, 0.9f);

        gen_line_arrays();
        gen_circ_arrays();
        gen_buffers();
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
        board_scaling = glm::scale(glm::mat4(1.0f),
                                   glm::vec3(0.9 * win_height / win_width, 0.9, 1.0));
    }
    void setMousePress(double x, double y)
    {
        // calculate intended move
        double fac = 1 / 0.9 / win_height;
        glm::vec2 mousePos = glm::scale(glm::mat4(1.0f), glm::vec3(fac, fac, 1.0)) *
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
    }
    void setCancelLastMove()
    {
        if (!go_game.cancel_last_qstate())
        {
            std::cout << "Cancel failed: Game start reached." << std::endl;
            return;
        }
        UPDATE_GUI = true;
    }

    // handles drawing
    // -------------------------------------------------------------
    void render()
    {
        // update gui if necessary
        if (UPDATE_GUI)
            update_board();

        // clearing before each render
        glClearColor(backColor.x, backColor.y, backColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering board
        shaderProgram.use();
        draw_lines();
        draw_stones();
    }

    // handles destruction
    // -------------------------------------------------------------
    ~BoardGUI()
    {
        glDeleteVertexArrays(1, &lVAO);
        glDeleteBuffers(1, &lVBO);
        glDeleteVertexArrays(1, &cVAO);
        glDeleteBuffers(1, &cVBO);
        glDeleteBuffers(1, &cEBO);
    }
};

#endif