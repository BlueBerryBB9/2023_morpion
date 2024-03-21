#pragma once
#include <array>
#include <optional>

class IPlayer {
public:
    IPlayer()          = default;
    virtual ~IPlayer() = default;

    virtual void                        set_win(char player)           = 0;
    virtual void                        set_draw()                     = 0;
    virtual std::optional<unsigned int> get_move(char player)          = 0;
    virtual void                        set_player_symbol(char player) = 0;
    virtual void set_board_state(const std::array<char, 9> &board)     = 0;
    virtual bool is_done()                                             = 0;
};
