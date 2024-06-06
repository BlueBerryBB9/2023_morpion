#include "GfxPlayer.hpp"
#include <optional>

GfxPlayer::GfxPlayer(char sym)
    : _win{sf::VideoMode(300, 350), "Tic Tac Toe"},
      _grid_text_changed{true},
      _status_text_changed{true},
      _sym{sym}
{
    _win.setFramerateLimit(10);
    _grid_text.draw_on(_win);
    _grid_lines.draw_on(_win);
    _status_text.set_text("hello :)");
    _status_text.draw_on(_win);
}

GfxPlayer::~GfxPlayer()
{
    if (_win.isOpen()) {
        _win.close();
    }
}

void GfxPlayer::set_win()
{
    _status_text.set_text(std::string{"winner: "} + _sym);
    _update_window_if_needed();
}

void GfxPlayer::set_draw()
{
    _status_text.set_text("no one wins");
    _update_window_if_needed();
}

void GfxPlayer::set_lose()
{
    _status_text.set_text("You lose !");
    _update_window_if_needed();
}

void GfxPlayer::_update_window_if_needed()
{
    _win.clear();
    _grid_lines.draw_on(_win);
    _grid_text.draw_on(_win);
    _status_text.draw_on(_win);
    _status_text_changed = false;
    _grid_text_changed   = false;
    _win.display();
}

std::optional<unsigned int> GfxPlayer::get_move()
{
    return (_move_made ? _move_made : std::nullopt);
}

void GfxPlayer::set_board_state(const std::array<char, 9> &board)
{
    _grid_text.set_from(board);
    _update_window_if_needed();
}

void GfxPlayer::process_events()
{
    sf::Event event;

    _move_made.reset();
    while (_win.pollEvent(event)) {
        if (event.type == sf::Event::Closed
            || (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape)) {
            _win.close();
            return;
        }
        if (_is_its_turn) {
            _move_made.reset();
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int grid_idx = event.mouseButton.x / 100
                               + (event.mouseButton.y / 100) * 3;
                // std::cout << "idx: " << grid_idx << std::endl;
                if (grid_idx > 8) {
                    _status_text.set_text("please click on the grid");
                } else {
                    // _status_text.set_text("");
                    _move_made = grid_idx;
                }
                _update_window_if_needed();
            }
        }
    }
    return;
}

bool GfxPlayer::is_done()
{
    return !_win.isOpen();
}

void GfxPlayer::ask_for_move()
{
    _status_text.set_text(std::string{"your turn: "} + _sym);
    _update_window_if_needed();
}

void GfxPlayer::set_turn(bool your_turn)
{
    _is_its_turn = your_turn;
}

void GfxPlayer::swap_turn()
{
    _move_made.reset();
    _is_its_turn = !_is_its_turn;
}

void GfxPlayer::set_player_symbol()
{
    _status_text.set_text(std::string{"you are player: "} + _sym);
    _update_window_if_needed();
}

char GfxPlayer::get_sym()
{
    return _sym;
}

void GfxPlayer::set_sym(char sym)
{
    _sym = sym;
}

void GfxPlayer::wait()
{
    _status_text.set_text(std::string{"Waiting another player..."});
    _update_window_if_needed();
}
