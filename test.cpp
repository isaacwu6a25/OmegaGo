// testing of main.cpp
#include <iostream>
#include "engine2.h"

using namespace std;

int main(int argc, char* argv[]) {

    GameEngine game(0);
    game.print_qstate(*(game.get_game_hist()[0]));

    qstate_ptr new_state1, new_state2;
    board_t filter;
    new_state1 = game.get_new_qstate(0, *(game.get_game_hist()[0]));
    
    game.print_qstate(*new_state1);
    new_state2 = game.get_new_qstate(1, *new_state1);
    game.print_qstate(*new_state2);
    return 0;
}