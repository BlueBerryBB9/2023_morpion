#pragma once
#include <array>
#include <optional>

class IPlayer {
public:
    IPlayer()          = default;
    virtual ~IPlayer() = default;

    virtual void set_win() = 0;
    // Server: "[SET_WIN]"

    virtual void set_draw() = 0;
    // Server: "[SET_DRAW]"

    virtual void set_lose() = 0;
    // Server: "[SET_LOSE]"

    virtual std::optional<unsigned int> get_move() = 0;

    virtual void set_player_symbol() = 0;
    // Server: "[SET_PLAYER_SYMBOL]"

    virtual void set_board_state(const std::array<char, 9> &board) = 0;
    // Server: "[SET_BOARD_STATE " + nine characters following + "]"
    // (e.g. : "[SET_BOARD_STATE x.o..oxx.]")

    virtual bool is_done() = 0;

    virtual void ask_for_move() = 0;
    // Server: "[ASK_FOR_MOVE]"

    virtual void set_turn(bool yout_turn) = 0;
    // Server: "[SET_TURN " + "0]" or "1]"

    virtual void swap_turn() = 0;
    // Server: "[SWAP_TURN]"

    virtual void process_events() = 0;
    // Client: "[MOVE " + (number between 0 & 8) + "]"

    virtual char get_sym() = 0;
};
