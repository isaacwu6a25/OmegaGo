// engine.h
#pragma once

#include <array>
#include <vector>
#include "definitions.h"
#include "gamestate.h"

typedef std::shared_ptr<GameState> state_ptr;

class GameEngine {

    // whole game history
    std::vector<state_ptr> game_hist;

    // possible moves for current player
    board_t legal_filter;

    // rules
    static int search_map[TOTAL_SIZE][4];   // 4 directions
    static std::array<std::array<int, 4>, 
                    TOTAL_SIZE> search_map_;
    void fill_search_map(void);
    bool valid_without_rules(state_ptr game_state, move_t move);
    

public:
    GameEngine(int komi);

    // gets all moves that can be taken
    board_ptr gen_legal_filter(state_ptr game_state);

    // updates state
    state_ptr gen_new_state(state_ptr game_state, move_t moves);
};