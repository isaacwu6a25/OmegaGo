//gamestate.cpp
#include <iostream>

#include "gamestate.h"

// creates game starting state
GameState::GameState(int komi) {

    // only even start supported atm
    if (komi == 0) {
        reset_board(&curr_board);
    }

}

// empties board
void GameState::reset_board(board_t* board) {
    for (int n=0; n<TOTAL_SIZE; n++) {
        (*board)[n] = EMPTY;
    }
}

void GameState::printboard(void) {
    for (int n=0; n<TOTAL_SIZE; n++) {
        std::cout << curr_board[n] << " ";
        if (n % BOARD_SIZE == 0) {
            std::cout << std::endl;
        }
    }
}