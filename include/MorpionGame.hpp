#pragma once
#include <array>

class MorpionGame {
public:
    enum class StartWith { PX, PO };
    MorpionGame(StartWith start_player = StartWith::PX);

    enum class Status : int { PXTurn, POTurn, PXWin, POWin, Draw };
    static constexpr char P1_CHAR = 'x';
    static constexpr char P2_CHAR = 'o';

    bool play(char player, unsigned int spot);
    void dump() const;

    const std::array<char, 9> array() const
    {
        return _grid;
    }

    Status status() const
    {
        return (_status);
    }

    bool done() const
    {
        return (_status != Status::POTurn && _status != Status::PXTurn);
    }

    void reset();

private:
    bool check_win_for(char player) const;
    bool check_for_draw() const;

    std::array<char, 9> _grid;
    Status              _status;
};
