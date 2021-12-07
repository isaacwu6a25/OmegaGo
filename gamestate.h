//gamestate.h
#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <array>
#include <vector>

//board #defines
#define BOARD_SIZE  19
#define TOTAL_SIZE  BOARD_SIZE * BOARD_SIZE
#define BLACK       0
#define WHITE       1
#define EMPTY       -1

//game #defines
#define READ_BACK   4

typedef bool player_t;
typedef std::array<int, TOTAL_SIZE> board_t;

class GameState {

    // 4+1 board states
    board_t curr_board;
    std::array<board_t, READ_BACK> past_boards;

    // player
    player_t player;

    // empties board
    void reset_board(board_t* board);

public:
    // constructors
    GameState(int komi);            // creates game start
    GameState(GameState* state);    // copies existing state

    void printboard(void);
    
};

#endif