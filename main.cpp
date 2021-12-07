// my_program.cpp
#include <iostream>
#include "gamestate.h"

using namespace std;

int main(int argc, char* argv[]) {

    GameState state = GameState(0);
    state.printboard();

    return 0;
}