// game.h
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <array>
#include <vector>

// board #defines
#define SIZE                19
#define NUM_INTERSECTIONS   SIZE * SIZE
#define BLACK               0
#define WHITE               1
#define EMPTY               -1
#define CROSS_NUM           4

// game #defines
#define KOMI                7.5
#define PASS                -1
#define RESIGN              -2
#define START               -3
#define NO_CHI              0

// output #defines
#define RECALL_MOVES        5
#define BLACK_LAYER         0
#define WHITE_LAYER         1
#define PLAYER_LAYER        2 * RECALL_MOVES


typedef int move_t;
typedef int pos_t;      //move_t includes pass and resign
typedef int color_t;
typedef std::array<int, NUM_INTERSECTIONS> board_t;

class GameState{

    public:
    color_t player;     // player who made the last move
    int move_num;
    move_t move;
    board_t board;
    // generate a state from current state
    void copy_state(GameState* game_state);
    color_t other_player(void);
};

class Game {

    public:
    // initiate game with empty board
    Game(void);

    // to make a move
    bool make_move(move_t move);

    // create output board state
    std::array<board_t, 11> output_state;
    void output_game_state(void);

    // 

    private:
    // state of game
    std::vector<GameState> game_hist;
    GameState next_state;

    // make sure move is placeable
    bool valid_without_rules(move_t move);

    // to search vertices
    int search_map[NUM_INTERSECTIONS][4];
    void fill_search_map(void);

    // generate a map of the current group and its liberties
    std::array<int, NUM_INTERSECTIONS> group_map;
    std::array<int, NUM_INTERSECTIONS> chi_map;
    void gen_chi_group_maps(pos_t pos, color_t color);
    void reset_chi_group_maps(void);
    int count_chi(void);

    // remove stones
    void remove_stones(std::array<int, NUM_INTERSECTIONS> &map);

    // test whether position has been seen before
    bool simple_ko(void);
};

#endif