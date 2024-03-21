#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>
#include "IPlayer.hpp"
#include "gfx/GridLines.hpp"
#include "gfx/TextPieces.hpp"
#include "gfx/TextStatus.hpp"

class GfxPlayer : public IPlayer {
public:
    GfxPlayer();
    ~GfxPlayer();
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
    unsigned int _process_events();
    void         _update_window_if_needed();

    std::optional<unsigned int> _move_made;
    sf::RenderWindow            _win;
    TextPieces                  _grid_text;
    bool                        _grid_text_changed;
    GridLines                   _grid_lines;
    TextStatus                  _status_text;
    bool                        _status_text_changed;
    bool                        _is_its_turn{false};
};
