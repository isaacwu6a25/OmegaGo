// engine version 2
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
    int count_chi(const qstate_t &qstate, move_t move);

    // search map
    void fill_search_map(void);
    search_t search_map;

public:
    // constructor
    GameEngine(int komi);

    // advance to new state
    qstate_ptr get_new_qstate(move_t move, 
                    const qstate_t &qstate);

    // create the rule filter
    void get_rule_filter(const qstate_t &qstate, 
                                board_t &filter);

    // for debugging
    const std::vector<qstate_ptr> &get_game_hist(void);
    void print_qstate(const qstate_t &qstate);
    void print_board(const board_t &board);
};

