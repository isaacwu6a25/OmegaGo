// engine2.h
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

#define READ_BACK       4
#define NNI_LAYERS      (READ_BACK + 1) * 2 + 1

typedef bool                            player_t;
typedef int                             move_t;
typedef std::array<bool, TOTAL_SIZE>    board_t;

typedef std::array<board_t, NNI_LAYERS> qstate_t;
typedef std::shared_ptr<qstate_t>       qstate_ptr;

class GameEngine {

    // the history of the current game
    std::vector<qstate_t> game_hist;

public:
    // advance state
    qstate_ptr get_new_qstate(board_t& filter, move_t move,
                            const qstate_t& qstate);


};

