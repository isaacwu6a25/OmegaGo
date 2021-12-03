// game.h
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

using namespace std;

// board #defines
#define SIZE                19
#define NUM_INTERSECTIONS   SIZE * SIZE
#define BLACK               0
#define WHITE               1
#define EMPTY               -1

// game #defines
#define KOMI                7.5
#define PASS                -1
#define RESIGN              -2
#define START               -3
#define n2posx(n)           (n+1)/SIZE
#define n2poxy(n)           (n+1)%SIZE

typedef int move_t;
typedef std::array<int, NUM_INTERSECTIONS> board_t;

struct GameState{
    int player;
    int move_num;
    int move;
    board_t board;
};

class Game {

    public:
    std::vector<GameState> game_hist;

    // initiate game with empty board
    Game(void);
    // to make a move
    bool make_move(move_t move);

    private:
    bool valid_without_rules(move_t move);

    bool liberty_rules(move_t move);
    bool has_air(move_t move);
    bool is_suicide(move_t move);

    bool simple_ko(move_t move);
};

#endif