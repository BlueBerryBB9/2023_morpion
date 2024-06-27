#include "MorpionGame.hpp"
#include <iostream>

MorpionGame::MorpionGame(MorpionGame::StartWith start_player)
    : _grid{},
      _status{(start_player == StartWith::PO) ? (Status::POTurn)
                                              : (Status::PXTurn)}
{
    _grid.fill('.');
}

bool MorpionGame::play(char player, unsigned int spot)
{
    Status next_st_turn;
    Status next_st_win;

    if (_status == Status::PXTurn && player == P1_CHAR) {
        next_st_turn = Status::POTurn;
        next_st_win  = Status::PXWin;
    } else if (_status == Status::POTurn && player == P2_CHAR) {
        next_st_turn = Status::PXTurn;
        next_st_win  = Status::POWin;
    } else {
        std::cerr << "not " << player << "'s turn" << std::endl;
        return false;
    }
    if (spot > _grid.size()) {
        std::cerr << "spot is outside of the grid " << spot << std::endl;
        return false;
    }
    if (_grid[spot] != '.') {
        std::cerr << "spot " << spot << " not free" << std::endl;
        return false;
    }
    _grid[spot] = player;
    if (check_win_for(player)) {
        _status = next_st_win;
    } else if (check_for_draw()) {
        _status = Status::Draw;
    } else {
        _status = next_st_turn;
    }
    return true;
}

void MorpionGame::dump() const
{
    std::cout << "MorpionGame::dump()" << std::endl;
    switch (_status) {
    case Status::PXTurn:
        std::cout << "PXTurn" << std::endl;
        break;
    case Status::POTurn:
        std::cout << "POTurn" << std::endl;
        break;
    case Status::PXWin:
        std::cout << "PXWin" << std::endl;
        break;
    case Status::POWin:
        std::cout << "POWin" << std::endl;
        break;
    case Status::Draw:
        std::cout << "Draw" << std::endl;
        break;
    }
    std::cout << _grid[0] << " " << _grid[1] << " " << _grid[2] << std::endl;
    std::cout << _grid[3] << " " << _grid[4] << " " << _grid[5] << std::endl;
    std::cout << _grid[6] << " " << _grid[7] << " " << _grid[8] << std::endl;
}

static bool check_3_spots(const std::array<char, 9> &grid, char val, int a,
                          int b, int c)
{
    return (grid[a] == val && grid[b] == val && grid[c] == val);
}

bool MorpionGame::check_win_for(char player) const
{
    for (int i : {0, 1, 2}) {
        if (check_3_spots(_grid, player, i, i + 3, i + 6))
            return true;
    }
    for (int i : {0, 3, 6}) {
        if (check_3_spots(_grid, player, i, i + 1, i + 2))
            return true;
    }
    if (check_3_spots(_grid, player, 0, 4, 8)
        || check_3_spots(_grid, player, 2, 4, 6))
        return (true);
    return (false);
}

bool MorpionGame::check_for_draw() const
{
    for (char piece : _grid) {
        if (piece == '.')
            return (false);
    }
    return (true);
}

void MorpionGame::reset()
{
    _grid   = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
    _status = Status::PXTurn;
}
