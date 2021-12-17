// engine version 2

#include <iostream>
#include "engine2.h"

using std::cout;
using std::endl;

// constructor
GameEngine::GameEngine(int komi) {

    fill_search_map();
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

    // copy old state and make the move
    player_t player;
    (*new_qstate)[0] = qstate[0]; 
    (*new_qstate)[READ_BACK + 1] = qstate[READ_BACK + 1];
    if (move != PASS) {
        if ((player = next_player(qstate)) == BLACK) {
            (*new_qstate)[0][move] = NEMPTY;
        } else {
            (*new_qstate)[READ_BACK + 1][move] = NEMPTY;
        }

        // TODO:: remove dead stones
        
        // update next player
        (*new_qstate)[NNI_LAYERS - 1].fill(!player);
    }

    return new_qstate;
}

// checks validity of proposed move
bool GameEngine::is_valid(const qstate_t &qstate, move_t move) {
    
    board_t new_self, new_opp;

    // PASS check
    if (move == PASS) {
        return true;
    }

    // overlapping stone check
    if (qstate[0][move] == NEMPTY || 
        qstate[READ_BACK + 1][move] == NEMPTY) {
        return false;
    }

    // liberty check
    move_t test_move; 
    bool valid = false;     // track validity in this section
    board_t stone_map;

    if (next_player(qstate) == BLACK) {
        new_self = qstate[0]; new_opp = qstate[READ_BACK + 1];
    } else {
        new_self = qstate[READ_BACK + 1]; new_opp = qstate[0];
    }
    new_self[move] = NEMPTY;

    // search opposing groups first
    for (int i = 0; i < CROSS_NUM; i++) {
        test_move = move + search_map[move][i];
        // continue if space empty
        if (new_self[test_move] == EMPTY && 
            new_opp[test_move]  == EMPTY) {
            // space exists therefore move is valid
            valid = true;
            continue;
        }
        // continue if space contains own color
        if (new_self[test_move] == NEMPTY) {
            continue;
        }
        /*  NOTE: new_opp and new_self are flipped,
            since currently testing for liberties 
            of groups of opposite player.
        */
        if (count_chi(new_opp, new_self, 
            stone_map, test_move) == 0) {
            // space exists therefore move is valid
            valid = true;
            remove_stones(new_opp, stone_map);
        }
    }
    // search own group if no spaces around move
    if (!valid && count_chi(new_self, 
        new_opp, stone_map, move) == 0) {
        return false;
    }

    // KO check -- TODOOO

    // passed all checks
    return true;
}

/*  Takes the updated board states (both current player and 
    opponent), the proposed move, as well as the stone map 
    as arguments.
    Fills the stone map by marking the stones that belong 
    to the group connected to the move just played.
    Returns the number of liberties of the same group.
*/
int GameEngine::count_chi(const board_t &self, const board_t &opp, 
                                board_t &stone_map, move_t move) {
    int total_chi = 0;
    board_t chi_map = {};
    stone_map.fill(EMPTY);  // reset stone_map

    gen_chi_grp_maps(self, opp, move, chi_map, stone_map);
    for (auto it = chi_map.begin(); it != chi_map.end(); it++) {
        total_chi += *it;
    }
    return total_chi;
}

/*  Takes the updated board states (both current player and 
    opponent), the proposed move, as well as the chi and 
    stone maps as arguments. 
    Fills the stone map by marking the stones that belong 
    to the group connected to the move just played. Fills
    the chi map by marking the liberties of the same group.
*/
void GameEngine::
    gen_chi_grp_maps(const board_t &self, const board_t &opp,
            move_t move, board_t &chi_map, board_t &stone_map) {

    move_t new_move;

    // position already passed or contains stone of opp color
    if (stone_map[move] == NEMPTY ||
        opp[move]       == NEMPTY) {
        return;
    }
    // position is empty
    if (self[move] == EMPTY) {
        // since it didn't satisfy the last conditional
        chi_map[move] = 1;
        return;
    }
    // mark the stone as part of the group
    stone_map[move] = NEMPTY;
    // recursively search around the poinnt clockwise
    for (int i = 0; i < CROSS_NUM; i++) {
        // check whether point needs to be searched
        if (!search_map[move][i]) {
            continue;
        }
        // get pos and check
        new_move = move + search_map[move][i];
        gen_chi_grp_maps(self, opp, new_move, chi_map, stone_map);
    }
}

/*  Takes a board state and stone map as arguments.
    Removes the stones as marked by the stone map from the board
    state.
*/
void GameEngine::remove_stones(board_t &board, const board_t &stone_map) {
    for (int n = 0; n < TOTAL_SIZE; n++) {
        if (stone_map[n] == NEMPTY) {
            board[n] = EMPTY;
        }
    }
}

// gets the player for next turn
player_t GameEngine::next_player(const qstate_t &qstate) {
    return qstate[NNI_LAYERS - 1][0];
}

// search map
void GameEngine::fill_search_map(void) {
    int x, y, std_chi[CROSS_NUM] = {1, 19, -1, -19};

    for (int n = 0; n < TOTAL_SIZE; n++) {
        x = n % BOARD_SIZE; y = n / BOARD_SIZE;
        // general intersection
        for (int m = 0; m < CROSS_NUM; m++) {
            search_map[n][m] = std_chi[m];
        }
        // check for horizontal edge case
        if (x == BOARD_SIZE - 1) {
            search_map[n][0] = 0;
        } else if (x == 0) {
            search_map[n][2] = 0;
        }
        // check for vertical edge case
        if (y == BOARD_SIZE - 1) {
            search_map[n][1] = 0;
        } else if (y == 0) {
            search_map[n][3] = 0;
        }
    }
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
    return game_hist;
}

const qstate_t &GameEngine::get_curr_qstate(void) {
    return *(game_hist[0]);
}