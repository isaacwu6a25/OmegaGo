// board.cpp
#include "game.h" // header in local directory
#include <iostream> // header in standard library

Game::Game(void) {
    GameState init_state;
    // create the first state
    for (int i=0; i<init_state.board.size(); i++) {
        init_state.board[i] = EMPTY;
    }
    init_state.move_num = 0;
    init_state.move = START;
    init_state.player = WHITE;
    game_hist.push_back(init_state);

    // create second state
    next_state.copy_state(&init_state);
    next_state.move_num++;
    next_state.player = init_state.other_player();

    fill_search_map();
    reset_chi_group_maps();
}

// make the move if is valid
bool Game::make_move(move_t move) {
    pos_t testpos;
    bool valid = false;

    if (!valid_without_rules(move)) {
        return false;
    }

    // remove captured groups
    // search around the point clockwise
    for (int i=0; i<CROSS_NUM; i++) {
        testpos = move + search_map[move][i];
        // move on if space empty
        if (next_state.board[testpos] == EMPTY) {
            // space exists therefore move is valid
            valid = true;
            continue;
        }
        // move on if space contains own color
        if (next_state.board[testpos] == next_state.player) {
            continue;
        }
        // generate group and chi maps
        gen_chi_group_maps(testpos, next_state.other_player());
        // test whether group has any liberties left
        if (count_chi() == NO_CHI) {
            // space exists therefore move is  valid
            valid = true;
            remove_stones(group_map);
        }
        // reset group and chi maps
        reset_chi_group_maps();
    }

    // search the group of the placed stone
    if (!valid) {
        // generate group and chi maps
        gen_chi_group_maps(move, next_state.player);
        // test whether group has any liberties left
        if (count_chi() == NO_CHI) {
            return false;
        }
    }
    return true;
}

// test whether move is on board and not overlapping stones
bool Game::valid_without_rules(move_t move) {
    // test if move is on board
    if (move < 0 || move >= NUM_INTERSECTIONS) {
        return false;
    }
    // test if board position is empty
    if (game_hist.back().board[move] != EMPTY) {
        return false;
    }
    return true;
}

// generate a map of the current group and its liberties
void Game::gen_chi_group_maps(pos_t pos, color_t color) {
    pos_t new_pos;
    
    // already passed this position
    if (group_map[pos] != EMPTY) {
        return;
    }
    // position is empty +1 chi
    if (next_state.board[pos] == EMPTY) {
        chi_map[pos] = 1;
        return;
    }
    // position has stone of opposite color
    if (next_state.board[pos] != color) {
        return;
    }

    // mark the stone as part of the group
    group_map[pos] = color;
    // recursively search around the point clockwise
    for (int i=0; i<CROSS_NUM; i++) {
        if (!search_map[pos][i]) {
            continue;
        }
        new_pos = pos + search_map[pos][i];
        gen_chi_group_maps(new_pos, color);
    }
    return;
}

// reset chi and group maps
void Game::reset_chi_group_maps(void) {
    group_map.fill(EMPTY);
    chi_map.fill(NO_CHI);
}

// count number of liberties
int Game::count_chi(void) {
    int sum = 0;
    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        sum += chi_map[n];
    }
    return sum;
}

// remove stones
void Game::remove_stones(std::array<int, NUM_INTERSECTIONS> &map) {
    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        if (map[n]) {
            next_state.board[n] = EMPTY;
        }
    }
}

// test whether position has been seen before
bool Game::simple_ko(void) {
    int diffs;

    for (int t=0; t<game_hist.size(); t++) {
        // assume the same
        diffs = 0;
        // count number of differences of board state
        for (int n=0; n<NUM_INTERSECTIONS; n++) {
            diffs += (game_hist[t].board[n] != next_state.board[n]);
        }
        // no differences
        if (diffs == 0) {
            return false;
        }
    }
    return true;
}

void Game::fill_search_map(void) {
    int x, y, std_chi[CROSS_NUM] = {1, 19, -1, -19};

    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        for (int m=0; m<CROSS_NUM; m++) {
        search_map[n][m] = std_chi[m];
        }
        x = n%SIZE; y = n/SIZE;
        if (x==SIZE-1) {
        search_map[n][0] = 0;
        } else if (x==0) {
        search_map[n][2] = 0;
        }
        if (y==SIZE-1) {
        search_map[n][1] = 0;
        } else if (y==0) {
        search_map[n][3] = 0;
        }
    }
}

void Game::output_game_state(void) {

}

void GameState::copy_state(GameState* game_state) {
    player = game_state->player;
    move_num = game_state->move_num;
    move = game_state->move;
    for (int n=0; n<NUM_INTERSECTIONS; n++) {
        board[n] = game_state->board[n];
    }
}

color_t GameState::other_player(void) {
    return (player + 1) % 2;
}