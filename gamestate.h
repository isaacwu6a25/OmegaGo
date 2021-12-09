//gamestate.h
#pragma once

#include <array>
#include <vector>
#include "definitions.h"

//board #defines
#define BOARD_SIZE  19
#define TOTAL_SIZE  BOARD_SIZE * BOARD_SIZE
#define BLACK       false
#define WHITE       true
#define EMPTY       false
#define NEMPTY      true
typedef std::array<bool, TOTAL_SIZE> board_t;

//game #defines
#define READ_BACK   4
typedef bool player_t;
typedef int move_t;

// neural #defines
#define OUTPUT_SIZE (READ_BACK) * 2 + 1
typedef std::array<board_t, OUTPUT_SIZE> bundle_t;
typedef std::array<bool, TOTAL_SIZE+1> nn_out_t;

// pointer type definitions
typedef std::shared_ptr<board_t> board_ptr;

class GameState {

    /* (4+1) x2 board states + 1 player state
        0-4: black current and past moves
        5-9: white current and past moves
        10: player bit
    */
    bundle_t bundle;

    // empties board
    void reset_board(board_t& board);

public:
    // constructor
    GameState(int komi);

    // returns output
    bundle_t* get_output(void);

    player_t get_next_player(void);

    // ---- debugging functions below ----

    void printboard(const board_t& board);
    board_t* get_curr_board_black(void);
    board_t* get_curr_board_white(void);
};
