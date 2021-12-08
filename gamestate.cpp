//gamestate.cpp
#include <iostream>
#include "gamestate.h"

/* ------------------------------------------------------------ */

// creates game starting state
GameState::GameState(int komi) {

    // no handicap
    if (komi == 0) {

        // empty all boards
        for (int i=0; i<OUTPUT_SIZE-1; i++) {
            reset_board(bundle[i]);
        }
        // set next player to black
        bundle[OUTPUT_SIZE-1].fill(BLACK);
    }
}

// returns output
bundle_t* GameState::get_output(void) {
    return &bundle;
}

// empties board
void GameState::reset_board(board_t& board) {
    board.fill(EMPTY);
}

/* everything below this is for debugging --------------------- */


// returns player of next turn
player_t GameState::get_next_player(void) {
    return bundle[OUTPUT_SIZE-1][0];
}

// prints board
void GameState::printboard(const board_t& board) {
    for (int n=0; n<TOTAL_SIZE; n++) {
        std::cout << board[n] << " ";
        if ((n + 1) % BOARD_SIZE == 0) {
            std::cout << std::endl;
        }
    }
}

// returns address of current boards
board_t* GameState::get_curr_board_black(void) {
    return &bundle[BLACK];
}
board_t* GameState::get_curr_board_white(void) {
    return &bundle[WHITE];
}