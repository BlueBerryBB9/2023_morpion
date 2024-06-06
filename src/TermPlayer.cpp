#include "TermPlayer.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <optional>
#include <sstream>

TermPlayer::~TermPlayer()
{
    if (_future.valid())
        std::cout << "Press enter for reader to stop" << std::endl;
}

void TermPlayer::set_win()
{
    std::cout << "Player " << _sym << " wins!" << std::endl;
}

void TermPlayer::set_draw()
{
    std::cout << "No one wins" << std::endl;
}

void TermPlayer::set_lose()
{
    std::cout << "You lose !" << std::endl;
}

std::optional<unsigned int> TermPlayer::get_move()
{
    return (_move_made ? _move_made : std::nullopt);
}

void TermPlayer::process_events()
{
    _move_made.reset();
    if (!_is_its_turn)
        return;
    if (_can_ask_again)
        ask_for_move();
    if (_future.wait_for(std::chrono::milliseconds(100))
        == std::future_status::ready) {
        _move_made     = _future.get();
        _can_ask_again = true;
    }
}

void TermPlayer::set_board_state(const std::array<char, 9> &board)
{
    unsigned int rounds{0};
    unsigned int idx{0};

    while (rounds < 3) {
        std::cout << board[idx];
        if (idx == 2 || idx == 5 || idx == 8) {
            rounds += 1;
            std::cout << std::endl;
        }
        idx += 1;
    }
}

bool TermPlayer::is_done()
{
    return !std::cin;
}

void TermPlayer::ask_for_move()
{
    std::cout << "Your turn: " << _sym << std::endl;
    std::cout << "Index between 0~8: ";
    std::cin.clear();

    _future        = std::async(std::launch::async, [&] {
        int         ians{9};
        std::string answer;

        std::cin.clear();
        getline(std::cin, answer);
        std::stringstream ss(answer);

        if (!(ss >> ians))
            return 9;

        return ians;
    });
    _can_ask_again = false;
}

void TermPlayer::set_turn(bool your_turn)
{
    _is_its_turn = your_turn;
}

void TermPlayer::swap_turn()
{
    _move_made.reset();
    _is_its_turn = !_is_its_turn;
}

void TermPlayer::set_player_symbol()
{
    std::cout << "You are player: " << _sym << std::endl;
}

char TermPlayer::get_sym()
{
    return _sym;
}

void TermPlayer::set_sym(char sym)
{
    _sym = sym;
}
