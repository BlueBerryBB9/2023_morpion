#include "TermPlayer.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <cstdio>
#include <future>
#include <iostream>
#include <sstream>

TermPlayer::TermPlayer()
{}

TermPlayer::~TermPlayer()
{
    if (_future.valid())
        std::cout << "Press enter for reader to stop" << std::endl;
}

void TermPlayer::set_win(char player)
{
    std::cout << "Player " << player << " wins!" << std::endl;
}

void TermPlayer::set_draw(void)
{
    std::cout << "No one wins" << std::endl;
}

std::optional<unsigned int> TermPlayer::get_move(char player)
{
    if (is_done())
        return {};

    if (!_is_its_turn)
        return {};

    if (_future.wait_for(std::chrono::milliseconds(100))
        == std::future_status::ready) {
        _move_made = _future.get();

        _can_ask_again = true;
        return _move_made;
    }

    return {};
}

void TermPlayer::set_player_symbol(char player)
{
    std::cout << "You are player: " << player << std::endl;
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
    return std::cin.eof();
}

void TermPlayer::ask_for_move(char sym)
{
    if (_can_ask_again) {
        std::cout << "Your turn: " << sym << std::endl;
        std::cout << "Index between 0~8: ";
        _future        = std::async(std::launch::async, [&] {
            int         ians{9};
            std::string answer;

            getline(std::cin, answer);
            std::stringstream ss(answer);
            if (!(ss >> ians)) {
                return 9;
            }

            return ians;
        });
        _can_ask_again = false;
    }
}

void TermPlayer::set_turn(bool your_turn)
{
    _is_its_turn = your_turn;
}

void TermPlayer::swap_turn()
{
    _is_its_turn = !_is_its_turn;
}
