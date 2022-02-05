// engine version 2

#include <iostream>
#include "engine2.h"

using std::cout;
using std::endl;

// GAME STATE CLASS ---------------------------------------------------
// constructor for GameState
GameState::GameState(const qstate_ptr &qstate)
{
    this_qstate = qstate;

    // count pieces
    black_num = count_pieces((*qstate)[0]);
    white_num = count_pieces((*qstate)[READ_BACK + 1]);
}

/*  Overloaded comparison operator, only checks the
    current state of the board, not the entire qstate.
*/
bool GameState::operator==(const GameState &rhs)
{
    // check piece numbers first
    return (black_num == rhs.black_num &&
            white_num == rhs.white_num &&
            (*this_qstate)[0] == (*rhs.this_qstate)[0] &&
            (*this_qstate)[READ_BACK + 1] == (*rhs.this_qstate)[READ_BACK + 1]);
}

// count pieces (obviously)
int GameState::count_pieces(const board_t &board)
{
    int count = 0;
    for (auto it = board.begin(); it != board.end(); it++)
    {
        count += *it;
    }
    return count;
}

// GAME ENGINE CLASS ---------------------------------------------------
// constructor for GameEngine
GameEngine::GameEngine(int handicap)
{
    // set up the env
    fill_search_map();

    // no handicap yet
    if (handicap == 0)
    {
        // qstate_ptr init_qstate(new qstate_t());
        auto init_qstate = std::make_shared<qstate_t>();
        game_hist.emplace_back(GameState(init_qstate));
    }
    else
    {
        cout << "Non-zero handicap is not supported." << endl;
    }
}

/*  Takes the proposed new qstate as the argument.
    Checks whether it repeats with any previous qstate.
    If valid, adds it to game history.
*/
int GameEngine::push_new_qstate(const qstate_ptr &qstate)
{
    GameState new_game_state(qstate);
    // test for repeat states
    // TODOOOOO
    // for (auto it = game_hist.begin(); it != game_hist.end(); it++)
    // {
    //     if (*it == new_game_state)
    //     {
    //         return false;
    //     }
    // }
    // add to game hist
    game_hist.emplace_back(new_game_state);
    return true;
}

// redo move
int GameEngine::cancel_last_qstate()
{
    if (game_hist.size() > 1)
    {
        game_hist.pop_back();
        return true;
    }
    return false;
}

/*  Takes the proposed move and the current qstate as
    arguments.
    If move is valid, returns a pointer to the new qstate.
    If move is invalid, returns nullptr.
*/
qstate_ptr GameEngine::get_new_qstate(move_t move,
                                      const qstate_t &qstate)
{
    player_t player = next_player(qstate);

    // create new board states
    board_t new_self, new_opp;

    // perform check if not PASS
    if (move != PASS)
    {
        // overlapping stone check
        if (qstate[0][move] == NEMPTY ||
            qstate[READ_BACK + 1][move] == NEMPTY)
        {
            return nullptr;
        }

        // assign self and opp boards
        if (player == BLACK)
        {
            new_self = qstate[0];
            new_opp = qstate[READ_BACK + 1];
        }
        else
        {
            new_self = qstate[READ_BACK + 1];
            new_opp = qstate[0];
        }
        new_self[move] = NEMPTY;

        // liberty check
        if (!chi_rules(new_self, new_opp, move))
        {
            return nullptr;
        }

        // KO check
        if (!ko_rules(new_self, new_opp, move, qstate))
        {
            return nullptr;
        }
    }
    // no checks required for PASS
    else
    {
        // assign self and opp boards
        if (player == BLACK)
        {
            new_self = qstate[0];
            new_opp = qstate[READ_BACK + 1];
        }
        else
        {
            new_self = qstate[READ_BACK + 1];
            new_opp = qstate[0];
        }
    }

    // create a new state
    auto new_qstate = std::make_shared<qstate_t>();

    // push back past moves
    for (int n = READ_BACK; n > 0; n--)
    {
        // all black boards are pushed back
        (*new_qstate)[n] = qstate[n - 1];
        // all white boards are pushed back
        (*new_qstate)[n + READ_BACK + 1] = qstate[n + READ_BACK];
    }

    // copy advanced board into new state
    if (player == BLACK)
    {
        (*new_qstate)[0] = new_self;
        (*new_qstate)[READ_BACK + 1] = new_opp;
    }
    else
    {
        (*new_qstate)[0] = new_opp;
        (*new_qstate)[READ_BACK + 1] = new_self;
    }

    // update next player
    (*new_qstate)[NNI_LAYERS - 1].fill(!player);

    return new_qstate;
}

/*  Takes in the new board states self and opp, as
    as well as the proposed move as arguments.
    Checks the validity of the move in terms of
    liberties.
    Removes dead stones from new_opp in the process.
*/
bool GameEngine::chi_rules(board_t &new_self,
                           board_t &new_opp, move_t move)
{
    move_t test_move;
    board_t stone_map;
    bool valid = false;

    // search opposing groups first
    for (int i = 0; i < CROSS_NUM; i++)
    {
        test_move = move + search_map[move][i];

        // continue if space empty
        if (new_self[test_move] == EMPTY &&
            new_opp[test_move] == EMPTY)
        {
            // space exists therefore move is valid
            valid = true;
            continue;
        }
        // continue if space contains own color
        if (new_self[test_move] == NEMPTY)
        {
            continue;
        }
        /*  NOTE: new_opp and new_self are flipped,
            since currently testing for liberties
            of groups of opposite player.
        */
        if (count_chi(new_opp, new_self,
                      stone_map, test_move) == 0)
        {
            // space exists therefore move is valid
            valid = true;
            remove_stones(new_opp, stone_map);
        }
    }
    // search own group if no spaces around move
    if (!valid && count_chi(new_self,
                            new_opp, stone_map, move) == 0)
    {
        return false;
    }

    // passed all checks
    return true;
}

/*  Takes the updated board states (both current player and
    opponent), the proposed move, as well as the stone map
    as arguments.
    Fills the stone map by marking the stones that belong
    to the group connected to the move just played.
    Returns the number of liberties of the same group.
*/
int GameEngine::count_chi(const board_t &self, const board_t &opp,
                          board_t &stone_map, move_t move)
{
    int total_chi = 0;
    board_t chi_map = {};
    stone_map.fill(EMPTY); // reset stone_map

    gen_chi_grp_maps(self, opp, move, chi_map, stone_map);
    for (auto it = chi_map.begin(); it != chi_map.end(); it++)
    {
        total_chi += *it;
    }
    return total_chi;
}

/*  Takes the updated board states (both current player and
    opponent), the proposed move, as well as the chi and
    stone maps as arguments.
    Fills the stone map by marking the stones that belong
    to the group connected to the move just played. Fills
    the chi map by marking the liberties of the same group.
*/
void GameEngine::
    gen_chi_grp_maps(const board_t &self, const board_t &opp,
                     move_t move, board_t &chi_map, board_t &stone_map)
{
    move_t new_move;

    // position already passed or contains stone of opp color
    if (stone_map[move] == NEMPTY ||
        opp[move] == NEMPTY)
    {
        return;
    }
    // position is empty
    if (self[move] == EMPTY)
    {
        // since it didn't satisfy the last conditional
        chi_map[move] = 1;
        return;
    }
    // mark the stone as part of the group
    stone_map[move] = NEMPTY;
    // recursively search around the poinnt clockwise
    for (int i = 0; i < CROSS_NUM; i++)
    {
        // check whether point needs to be searched
        if (!search_map[move][i])
        {
            continue;
        }
        // get pos and check
        new_move = move + search_map[move][i];
        gen_chi_grp_maps(self, opp, new_move, chi_map, stone_map);
    }
}

/*  Takes a board state and stone map as arguments.
    Removes the stones as marked by the stone map from the board
    state.
*/
void GameEngine::remove_stones(board_t &board, const board_t &stone_map)
{
    for (int n = 0; n < TOTAL_SIZE; n++)
    {
        if (stone_map[n] == NEMPTY)
        {
            board[n] = EMPTY;
        }
    }
}

/*  Takes the updated board states (current player and
    opponent) as well as curr qstate as arguments.
    Returns whether the proposed move satisfies ko-rules.
    Note: Only checks for repeats in the current qstate.
*/
bool GameEngine::ko_rules(board_t &new_self, board_t &new_opp,
                          move_t move, const qstate_t &qstate)
{
    // differentiate black and white turns
    if (next_player(qstate) == BLACK)
    {
        for (int i = 0; i < READ_BACK; i++)
        {
            if (qstate[i + 1][move] == NEMPTY &&
                qstate[i + 1] == new_self &&
                qstate[READ_BACK + i + 2] == new_opp)
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = 0; i < READ_BACK; i++)
        {
            if (qstate[READ_BACK + i + 2][move] == NEMPTY &&
                qstate[READ_BACK + i + 2] == new_self &&
                qstate[i + 1] == new_opp)
            {
                return false;
            }
        }
    }
    return true;
}

// gets the player for next turn
player_t GameEngine::next_player(const qstate_t &qstate)
{
    return qstate[NNI_LAYERS - 1][0];
}

// search map
void GameEngine::fill_search_map(void)
{
    int x, y, std_chi[CROSS_NUM] = {1, 19, -1, -19};

    for (int n = 0; n < TOTAL_SIZE; n++)
    {
        x = n % BOARD_SIZE;
        y = n / BOARD_SIZE;
        // general intersection
        for (int m = 0; m < CROSS_NUM; m++)
        {
            search_map[n][m] = std_chi[m];
        }
        // check for horizontal edge case
        if (x == BOARD_SIZE - 1)
        {
            search_map[n][0] = 0;
        }
        else if (x == 0)
        {
            search_map[n][2] = 0;
        }
        // check for vertical edge case
        if (y == BOARD_SIZE - 1)
        {
            search_map[n][1] = 0;
        }
        else if (y == 0)
        {
            search_map[n][3] = 0;
        }
    }
}

// utility functions
// -----------------------------------------------------------------
const std::vector<GameState> &GameEngine::get_game_hist(void)
{
    return game_hist;
}

qstate_t &GameEngine::get_curr_qstate(void)
{
    return *((game_hist.end() - 1)->this_qstate);
}

// for debugging ---------------------------------------------------
void GameEngine::print_qstate(const qstate_t &qstate)
{
    cout << "BEGIN QSTATE" << endl;
    for (int n = NNI_LAYERS - 1; n >= 0; n--)
    {
        cout << "L" << n << ": ";
        if (n < 5)
        {
            cout << "BLACK PAST MOVE " << n + 1 << endl;
        }
        else if (n != 10)
        {
            cout << "WHITE PAST MOVE " << n - READ_BACK << endl;
        }
        else
        {
            cout << "PLAYER LAYER" << endl;
        }
        print_board(qstate[n]);
    }
    cout << "END QSTATE\n"
         << endl;
}

void GameEngine::print_board(const board_t &board)
{
    for (auto it = board.begin(); it != board.end(); it++)
    {
        cout << *it << " ";
        if ((it - board.begin() + 1) % BOARD_SIZE == 0)
        {
            cout << endl;
        }
    }
}

void GameEngine::set_move(move_t move, player_t player)
{
    // assume the move is valid and not pass
    if (player == BLACK)
    {
        get_curr_qstate()[0][move] = NEMPTY;
    }
    else
    {
        get_curr_qstate()[READ_BACK + 1][move] = NEMPTY;
    }
    set_player(!player);
}

void GameEngine::set_player(player_t player)
{
    get_curr_qstate()[NNI_LAYERS - 1].fill(player);
}
