// engine.cpp

#include <iostream>
#include "definitions.h"
#include "engine.h"
#include "gamestate.h"

/* CONSTRUCTOR------------------------------------------------- */
// starts the game
GameEngine::GameEngine(int komi) {

    state_ptr init_state (new GameState(komi));
    game_hist.push_back(init_state);

    fill_search_map();
}

/* PUBLIC------------------------------------------------------ */
// gets all moves that can be taken
board_ptr GameEngine::gen_legal_filter(state_ptr game_state) {

    // create board
    board_ptr filter(new board_t);
}

// updates state
state_ptr GameEngine::gen_new_state(state_ptr game_state,
                                    move_t move) {
    // create new state
    state_ptr new_state(new GameState(game_state));

    // advance state
    for (int i=READ_BACK; i>0; i--) {
        (*new_state).get_output()[i] 
                    = (*new_state).get_output()[i-1];
        (*new_state).get_output()[i+READ_BACK+1] 
                    = (*new_state).get_output()[i+READ_BACK];
    }

    // register the move
    player_t player = (*new_state).get_next_player();
    if (player == BLACK) {
        (*new_state).get_output()[0][move] = NEMPTY;
    } else {
        (*new_state).get_output()[READ_BACK+1][move] = NEMPTY;
    }

    return new_state;
}

/* RULES------------------------------------------------------- */
// test if move is on board and not overlapping stones
bool GameEngine::valid_without_rules(state_ptr game_state, 
                                    move_t move) {
    // test if move is on board
    if (move < 0 || move >= TOTAL_SIZE) {
        return false;
    }
    // test if move is overlapping other stones
    if ((*game_state).get_output()[0][move] == NEMPTY || 
        (*game_state).get_output()[READ_BACK+1][move] == NEMPTY) {
        
        return false;
    }
    return true;
}

// fill the search map
void GameEngine::fill_search_map(void) {
    search_map_.fill((std::array<int, 4>) {1, 19, -1, -19});
    for (int i=0; i<TOTAL_SIZE; i++) {
        std::cout << search_map_[i][0] << " ";
        if ((i+1) % BOARD_SIZE == 0) {
            std::cout << std::endl;
        }
    }
}