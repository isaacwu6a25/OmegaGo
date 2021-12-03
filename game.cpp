// board.cpp
#include "game.h" // header in local directory
#include <iostream> // header in standard library
#include <array>
#include <vector>

using namespace std;

Game::Game(void) {
    struct GameState init_state;

    for (int i=0; i<init_state.board.size(); i++) {
        init_state.board[i] = EMPTY;
    }
    init_state.move_num = 0;
    init_state.move = START;
    init_state.player = WHITE;

    game_hist.push_back(init_state);
}

bool Game::make_move(move_t move) {

}

bool Game::valid_without_rules(move_t move) {
    if (move < RESIGN || move >= NUM_INTERSECTIONS) {
        return false;
    }
    if (game_hist.back().board[move] != EMPTY) {
        return false;
    }
    return true;
}

bool Game::has_air(move_t move) {
    int xpos, ypos;
    xpos = n2posx(move); ypos = n2poxy(move);

    if (xpos > 1 && xpos < SIZE && ypos > 1 && ypos < SIZE) {
        if (game_hist.back().board[move-1] != EMPTY ||
            game_hist.back().board[move+1] != EMPTY) {
        }
    }

    return false;
}


