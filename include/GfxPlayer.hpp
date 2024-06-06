#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>
#include "../src/gfx/GridLines.hpp"
#include "../src/gfx/TextPieces.hpp"
#include "../src/gfx/TextStatus.hpp"
#include "IPlayer.hpp"

class GfxPlayer : public IPlayer {
public:
    GfxPlayer(char sym);
    ~GfxPlayer();

    void set_win() override;
    void set_draw() override;
    void set_lose() override;
    void set_board_state(const std::array<char, 9> &board) override;
    bool is_done() override;
    void ask_for_move() override;
    void set_turn(bool your_turn) override;
    void swap_turn() override;
    void process_events() override;
    void set_player_symbol() override;
    char get_sym() override;
    void set_sym(char sym) override;
    std::optional<unsigned int> get_move() override;

private:
    void _update_window_if_needed();

    std::optional<unsigned int> _move_made{std::nullopt};
    sf::RenderWindow            _win;
    TextPieces                  _grid_text;
    bool                        _grid_text_changed;
    GridLines                   _grid_lines;
    TextStatus                  _status_text;
    bool                        _status_text_changed;
    bool                        _is_its_turn{false};
    char                        _sym;
};
