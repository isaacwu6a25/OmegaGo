// game.h
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <array>
#include <vector>

using namespace std;

// board #defines
#define SIZE                19
#define NUM_INTERSECTIONS   SIZE * SIZE
#define BLACK               1
#define WHITE               -1
#define EMPTY               0

// game #defines
#define KOMI                7.5
#define PASS                -1
#define RESIGN              -2
#define START               -3
#define n2posx(n)           (n+1)/SIZE
#define n2poxy(n)           (n+1)%SIZE

typedef int move_t;
typedef std::array<int, NUM_INTERSECTIONS> board_t;

class GameState{

    public:
    int player;
    int move_num;
    int move;
    board_t board;

    void copy_state(GameState* game_state);
};

class Game {

    public:
    std::vector<GameState> game_hist;

    // initiate game with empty board
    Game(void);

    // to make a move
    bool make_move(move_t move);

    private:
    // make sure move is placeable
    bool valid_without_rules(move_t move);

    // to search for liberties
    int chi_search[NUM_INTERSECTIONS][4];
    void fill_chi_search(void);

    // generate a map of liberties
    int chi_map[NUM_INTERSECTIONS];
    void gen_chi_map(move_t move);

    // generate a map of groups
    int group_map[NUM_INTERSECTIONS];
    void gen_group_map();
    void find_same_group();

    bool is_suicide(move_t move);

    bool simple_ko(move_t move);
};

#endif