#pragma once
#include <array>
#include <optional>

class IPlayer {
public:
    IPlayer()          = default;
    virtual ~IPlayer() = default;

    virtual void                        set_win()                  = 0;
    virtual void                        set_draw()                 = 0;
    virtual std::optional<unsigned int> get_move()                 = 0;
    virtual void                        set_player_symbol()        = 0;
    virtual void set_board_state(const std::array<char, 9> &board) = 0;
    virtual bool is_done()                                         = 0;
    virtual void ask_for_move()                                    = 0;
    virtual void set_turn(bool yout_turn)                          = 0;
    virtual void swap_turn()                                       = 0;
    virtual void process_events()                                  = 0;
    virtual char get_sym()                                         = 0;
};
