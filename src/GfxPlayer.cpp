#include "GfxPlayer.hpp"

GfxPlayer::GfxPlayer()
    : _win{sf::VideoMode(300, 350), "Tic Tac Toe"},
      _grid_text_changed{true},
      _status_text_changed{true}
{
    _win.setFramerateLimit(5);
    _grid_text.draw_on(_win);
    _grid_lines.draw_on(_win);
    _status_text.set_text("hello :)");
    _status_text.draw_on(_win);
}

void GfxPlayer::set_win(char player)
{
    _status_text.set_text(std::string{"winner: "} + player);
    _status_text_changed = true;
    _update_window_if_needed();
}

void GfxPlayer::set_draw(void)
{
    _status_text.set_text("no one wins");
    _status_text_changed = true;
    _update_window_if_needed();
}

void GfxPlayer::_update_window_if_needed()
{
    if (_grid_text_changed || _status_text_changed) {
        _win.clear();
        _grid_lines.draw_on(_win);
        _grid_text.draw_on(_win);
        _status_text.draw_on(_win);
        _status_text_changed = false;
        _grid_text_changed   = false;
        _win.display();
    }
}

std::optional<unsigned int> &GfxPlayer::get_move(char player)
{
    _status_text.set_text(std::string{"your turn: "} + player);
    _status_text_changed = true;
    _move_made.reset();
    while (_win.isOpen() && !_move_made) {
        this->_process_events();
        this->_update_window_if_needed();
    }
    return _move_made;
}

void GfxPlayer::set_player_symbol(char player)
{
    _status_text.set_text(std::string{"you are player: "} + player);
    _status_text_changed = true;
}

void GfxPlayer::set_board_state(const std::array<char, 9> &board)
{
    _grid_text.set_from(board);
    _grid_text_changed = true;
    _update_window_if_needed();
}

unsigned int GfxPlayer::_process_events()
{
    unsigned int n_processed{0};
    sf::Event    event;

    while (_win.pollEvent(event)) {
        if (event.type == sf::Event::Closed
            || (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape))
            _win.close();
        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            int grid_idx
                = event.mouseButton.x / 100 + (event.mouseButton.y / 100) * 3;
            std::cout << "idx: " << grid_idx << std::endl;
            if (grid_idx > 8) {
                _status_text.set_text("please click on the grid");
            } else {
                _status_text.set_text("");
                _move_made = grid_idx;
            }
            _status_text_changed = true;
        }
        n_processed += 1;
    }
    return n_processed;
}

bool GfxPlayer::is_done()
{
    return !_win.isOpen();
}
