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

class GameEngine {

    // the history of the current game
    std::vector<qstate_ptr> game_hist;

    // gets the player for next turn
    player_t next_player(const qstate_t &qstate);

    // rules
    int count_chi(const board_t &self, const board_t &opp, 
                        board_t & stone_map, move_t move);
    void gen_chi_grp_maps(const board_t &self, const board_t &opp, 
                move_t move, board_t &chi_map, board_t &stone_map);
    void remove_stones(board_t &board, const board_t &stone_map);

    // search map
    void fill_search_map(void);
    search_t search_map;

public:
    // constructor
    GameEngine(int komi);

    // advance to new state
    qstate_ptr get_new_qstate(move_t move, 
                    const qstate_t &qstate);

    // checks validity of proposed move
    bool is_valid(const qstate_t &qstate, move_t move);

    // for debugging
    const std::vector<qstate_ptr> &get_game_hist(void);
    const qstate_t &get_curr_qstate(void);
    void print_qstate(const qstate_t &qstate);
    void print_board(const board_t &board);
};

