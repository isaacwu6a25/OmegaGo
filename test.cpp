// testing of main.cpp
#include <iostream>
#include "engine2.h"

using namespace std;

int main(int argc, char *argv[])
{

    GameEngine game(0);
    qstate_ptr new_qstate1, new_qstate2, new_qstate3, new_qstate4;

    game.set_move(1, BLACK);
    game.set_move(3, BLACK);
    game.set_move(20, WHITE);
    game.set_move(22, WHITE);
    game.set_move(40, WHITE);
    game.set_move(21, BLACK);
    game.print_qstate(game.get_curr_qstate());
    new_qstate1 = game.get_new_qstate(2, game.get_curr_qstate());
    new_qstate2 = game.get_new_qstate(300, *new_qstate1);
    new_qstate3 = game.get_new_qstate(320, *new_qstate2);
    new_qstate4 = game.get_new_qstate(21, *new_qstate3);

    if (new_qstate4 != nullptr)
    {
        // game.print_qstate(*new_qstate4);
    }
    else
    {
        cout << "move is invalid" << endl;
    }

    game.push_new_qstate(new_qstate4);
    game.print_qstate(game.get_curr_qstate());
    return 0;
}