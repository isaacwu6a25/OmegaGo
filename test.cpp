// testing of main.cpp
#include <iostream>
#include "engine2.h"

using namespace std;

int main(int argc, char* argv[]) {

    GameEngine game(0);
    // game.print_qstate(*(game.get_game_hist()[0]));

    qstate_ptr new_state1, new_state2, new_state3, new_state4;
    new_state1 = game.get_new_qstate(39, game.get_curr_qstate());
    
    // game.print_qstate(*new_state1);
    new_state2 = game.get_new_qstate(1, *new_state1);
    new_state3 = game.get_new_qstate(21, *new_state2);
    new_state4 = game.get_new_qstate(60, *new_state3);
    game.print_qstate(*new_state4);
    game.is_valid(*new_state4, 40);
    return 0;
}