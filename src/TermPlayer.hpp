#pragma once
#include <optional>
#include "IPlayer.hpp"

class TermPlayer : public IPlayer {
public:
    TermPlayer();
    ~TermPlayer();
    void                        set_win(char player) override;
    void                        set_draw(void) override;
    std::optional<unsigned int> get_move(char player) override;
    void                        set_player_symbol(char player) override;
    void set_board_state(const std::array<char, 9> &board) override;
    bool is_done() override;
    void ask_for_move(char sym) override;
    void set_turn(bool your_turn) override;
    void swap_turn() override;

private:
    std::optional<unsigned int> _move_made;
    bool                        _is_its_turn;
};
