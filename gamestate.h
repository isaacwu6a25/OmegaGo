//gamestate.h
#pragma once

#include <array>
#include <vector>

//board #defines
#define BOARD_SIZE  19
#define TOTAL_SIZE  BOARD_SIZE * BOARD_SIZE
#define BLACK       0
#define WHITE       1
#define EMPTY       0

//game #defines
#define READ_BACK   4

// neural #defines
#define OUTPUT_SIZE (READ_BACK) * 2 + 1

typedef bool player_t;
typedef std::array<bool, TOTAL_SIZE> board_t;
typedef std::array<board_t, OUTPUT_SIZE> bundle_t;

class GameState {

    /* (4+1) x2 board states + 1 player state
        0: black current
        1: white current
        2-5: black past 1-4 moves
        6-9: white past 1-4 moves
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

    // ---- debugging functions below ----

    player_t get_next_player(void);
    void printboard(const board_t& board);
    board_t* get_curr_board_black(void);
    board_t* get_curr_board_white(void);
};
