// engine version 2

#include <iostream>
#include "engine2.h"

using std::cout;
using std::endl;

// constructor
GameEngine::GameEngine(int komi) {
    if (komi == 0) {
        qstate_ptr init_qstate(new qstate_t);
        game_hist.push_back(init_qstate);
    } else {
        cout << "Non-zero komi is not supported." << endl;
    }
}

// advance to new state: WARNING -- implements no checks
qstate_ptr GameEngine::get_new_qstate(move_t move, 
                            const qstate_t &qstate) {
    // create a new state
    qstate_ptr new_qstate(new qstate_t);

    // push back past moves
    for (int n = READ_BACK; n > 0; n--) {
        // all black boards are pushed back
        (*new_qstate)[n] = qstate[n-1];
        // all white boards are pushed back
        (*new_qstate)[n + READ_BACK + 1] = qstate[n + READ_BACK];
    }

    // copy and make the move
    player_t player;
    (*new_qstate)[0] = qstate[0]; 
    (*new_qstate)[READ_BACK + 1] = qstate[READ_BACK + 1];
    if (move != PASS) {
        if ((player = next_player(qstate)) == BLACK) {
            (*new_qstate)[0][move] = NEMPTY;
        } else {
            (*new_qstate)[READ_BACK + 1][move] = NEMPTY;
        }
    }

    // TODO:: remove dead stones
    
    // update next player
    (*new_qstate)[NNI_LAYERS - 1].fill(!player);

    return new_qstate;
}

// create the rule filter
void GameEngine::get_rule_filter(const qstate_t &qstate, 
                                board_t &filter) {
    // perform rule checking on every intersection
    for (int n = 0; n < TOTAL_SIZE; n++) {

        // check for overlapping stones
        if (qstate[0][n] == NEMPTY || 
            qstate[READ_BACK + 1][n] == NEMPTY) {
            filter[n] = NLEGAL;
            continue;
        }
        // check for liberties

        // passed all checks
        filter[n] = LEGAL;
    }
}

// rules
int GameEngine::count_chi(const qstate_t &qstate, move_t move) {
    int total_chi = 0;
    board_t chi_map, stone_map;


    return total_chi;
}

// gets the player for next turn
player_t GameEngine::next_player(const qstate_t &qstate) {
    return qstate[NNI_LAYERS - 1][0];
}

// search map
void fill_search_map(void) {
    
}

// for debugging ---------------------------------------------------
void GameEngine::print_qstate(const qstate_t &qstate) {
    for (int n = 0; n < NNI_LAYERS; n++) {
        cout << "L" << n << ": ";
        if (n < 5) {
            cout << "BLACK PAST MOVE " << n + 1 << endl;
        } else if (n!= 10) {
            cout << "WHITE PAST MOVE " << n - READ_BACK << endl;
        } else {
            cout << "PLAYER LAYER" << endl;
        }
        print_board(qstate[n]);
    }
}

void GameEngine::print_board(const board_t &board) {
    for (auto it = board.begin(); it != board.end(); it++) {
        cout << *it << " ";
        if ((it - board.begin() + 1) % BOARD_SIZE == 0) {
            cout << endl;
        }
    }
}

const std::vector<qstate_ptr> &GameEngine::get_game_hist(void) {
    const std::vector<qstate_ptr> &ref = game_hist;
    return ref;
}