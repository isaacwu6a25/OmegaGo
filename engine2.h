// engine version 2 header
#pragma once

#include <array>
#include <vector>
#include <memory>

#define BLACK           false
#define WHITE           true
#define EMPTY           false
#define NEMPTY          true
#define BOARD_SIZE      19
#define TOTAL_SIZE      BOARD_SIZE * BOARD_SIZE
#define PASS            -1

#define LEGAL           true
#define NLEGAL          false
#define CROSS_NUM       4
#define READ_BACK       4
#define NNI_LAYERS      (READ_BACK + 1) * 2 + 1

typedef bool                            player_t;
typedef int                             move_t;
typedef std::array<bool, TOTAL_SIZE>    board_t;
typedef std::array<std::array<int, 
            CROSS_NUM>, TOTAL_SIZE>     search_t;

typedef std::array<board_t, NNI_LAYERS> qstate_t;
typedef std::shared_ptr<qstate_t>       qstate_ptr;

class GameState {
    // piece numbers
    int white_num, black_num;

    // pointer to qstate
    qstate_ptr this_qstate;

    // constructor
    GameState(const qstate_ptr &qstate);
    int count_pieces(const board_t &board);

    // overloaded comparison
    bool operator == (const GameState &rhs);

    // the game can access everything within gamestate
    friend class GameEngine;
};


class GameEngine {

    // the history of the current game
    std::vector<GameState> game_hist;

    // gets the player for next turn
    player_t next_player(const qstate_t &qstate);

    // for checking liberty requirements
    bool chi_rules(board_t &new_self, board_t &new_opp, move_t move);
    int count_chi(const board_t &self, const board_t &opp, 
                        board_t & stone_map, move_t move);
    void gen_chi_grp_maps(const board_t &self, const board_t &opp, 
                move_t move, board_t &chi_map, board_t &stone_map);
    void remove_stones(board_t &board, const board_t &stone_map);

    // for checking KO requiremnts
    bool ko_rules(board_t &new_self, board_t &new_opp, 
                move_t move, const qstate_t &qstate);

    // search map
    void fill_search_map(void);
    search_t search_map;

public:
    // constructor
    GameEngine(int handicap);

    // advance to new state
    qstate_ptr get_new_qstate(move_t move, const qstate_t &qstate);
    int push_new_qstate(const qstate_ptr &qstate);

    // for debugging
    const std::vector<GameState> &get_game_hist(void);
    qstate_t &get_curr_qstate(void);
    void set_move(move_t move, player_t player);
    void set_player(player_t player);
    void print_qstate(const qstate_t &qstate);
    void print_board(const board_t &board);
};

