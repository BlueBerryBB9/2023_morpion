#include "GfxPlayer.hpp"
#include <optional>
#include "IPlayer.hpp"

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
    if (_win.isOpen())
        _win.close();
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
        if (_phase == PLAYER_PHASE::playing) {
            if (_is_its_turn) {
                if (event.type == sf::Event::MouseButtonPressed
                    && event.mouseButton.button == sf::Mouse::Left) {
                    int grid_idx = event.mouseButton.x / 100
                                   + (event.mouseButton.y / 100) * 3;
                    if (grid_idx > 8) {
                        _status_text.set_text("please click on the grid");
                        _update_window_if_needed();
                    } else {
                        _move_made = grid_idx;
                        std::cout << "MOVE " << _move_made.value() << std::endl;
                    }
                }
            }
        } else if (_phase == PLAYER_PHASE::replay) {
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int grid_idx = event.mouseButton.x / 100
                               + (event.mouseButton.y / 100) * 3;
                if (grid_idx != 3 && grid_idx != 5) {
                    _status_text.set_text(
                        "please click on the symbols : X (yes) / O (no)");
                    _update_window_if_needed();
                } else {
                    _move_made = (grid_idx == 3 ? 1 : 0);
                }
            }
        }
    }
}

bool GfxPlayer::is_done()
{
    return !_win.isOpen();
}

void GfxPlayer::ask_for_move()
{
    // if (_is_last_mv_ask_mv)
    //     return play_again();

    std::cout << "here2\n";
    if (_phase == PLAYER_PHASE::waiting_opponent) {
        std::cout << "here4444\n";
    }
    if (_phase == PLAYER_PHASE::playing) {
        std::cout << "here3\n";
        _status_text.set_text(std::string{"your turn: "} + _sym);
        _update_window_if_needed();
        // _is_last_mv_ask_mv = true;
    } else if (_phase == PLAYER_PHASE::replay) {
        _status_text.set_text(
            std::string{"Do you want to play again ? (yes = x / no = o)"});
        set_board_state({'.', '.', '.', 'x', '.', 'o', '.', '.', '.'});
    }
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
    // _is_last_mv_ask_mv = false;
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
    if (_phase == PLAYER_PHASE::replay) {
        _status_text.set_text(std::string{"Waiting for the other player..."});
        _update_window_if_needed();
        return;
    }
    _status_text.set_text(std::string{"Waiting another player..."});
    _update_window_if_needed();
}

void GfxPlayer::play_again()
{
    _status_text.set_text(std::string{"Play Again !"});
    _update_window_if_needed();
}

void GfxPlayer::set_phase(PLAYER_PHASE phase)
{
    _phase = phase;
}
