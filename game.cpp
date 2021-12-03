// board.cpp
#include "game.h" // header in local directory
#include <iostream> // header in standard library

using namespace std;

Game::Game(void) {
    GameState init_state;

    for (int i=0; i<init_state.board.size(); i++) {
        init_state.board[i] = EMPTY;
    }
    init_state.move_num = 0;
    init_state.move = START;
    init_state.player = WHITE;

    game_hist.push_back(init_state);

    fill_chi_search();
}

bool Game::make_move(move_t move) {
    GameState new_state;

    if (!valid_without_rules(move)) {
        return false;
    }
    new_state.copy_state(&game_hist.back());
}

// test whether move is on board and not overlapping stones
bool Game::valid_without_rules(move_t move) {
    if (move < RESIGN || move >= NUM_INTERSECTIONS) {
        return false;
    }
    if (game_hist.back().board[move] != EMPTY) {
        return false;
    }
    return true;
}

// generates a map of liberties
void Game::gen_chi_map(move_t move) {
    

    return;
}

void Game::fill_chi_search(void) {
    int x, y, std_chi[4] = {1, 19, -1, -19};

    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        for (int m=0; m<4; m++) {
            chi_search[n][m] = std_chi[m];
        }
        x = n%SIZE; y = n/SIZE;
        if (x==SIZE-1) {
            chi_search[n][0] = 0;
        } else if (x==0) {
            chi_search[n][2] = 0;
        }
        if (y==SIZE-1) {
            chi_search[n][1] = 0;
        } else if (y==0) {
            chi_search[n][3] = 0;
        }
    }
}

void GameState::copy_state(GameState* game_state) {
    player = game_state->player;
    move_num = game_state->move_num;
    move = game_state->move;
    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        board[n] = game_state->board[n];
    }
}