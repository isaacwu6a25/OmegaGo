// definitions.h
#pragma once

#include "gamestate.h"

//board #defines
#define BOARD_SIZE  19
#define TOTAL_SIZE  BOARD_SIZE * BOARD_SIZE
#define BLACK       false
#define WHITE       true
#define EMPTY       false
#define NEMPTY      true
typedef std::array<bool, TOTAL_SIZE> board_t;

//game #defines
#define READ_BACK   4
typedef bool player_t;
typedef int move_t;

// neural #defines
#define OUTPUT_SIZE (READ_BACK) * 2 + 1
typedef std::array<board_t, OUTPUT_SIZE> bundle_t;
typedef std::array<bool, TOTAL_SIZE+1> nn_out_t;

// pointer type definitions
typedef std::shared_ptr<board_t> board_ptr;