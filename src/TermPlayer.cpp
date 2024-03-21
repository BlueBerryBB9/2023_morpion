#include "TermPlayer.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

TermPlayer::TermPlayer()
{}

TermPlayer::~TermPlayer()
{}

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
    std::cout << "Your turn: " << player << std::endl;
    std::cout << "Index between 0~8: ";
    _move_made.reset();
    unsigned int answer;
    if (std::cin >> answer) {
        _move_made = answer;
    } else {
        // INFO: the 9 OOB move indicates to arena that move was invalid
        _move_made = 9;
        std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
    }
    return _move_made;
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
    return true;
}

void TermPlayer::ask_for_move(char sym)
{}

void TermPlayer::set_turn(bool your_turn)
{
    _is_its_turn = your_turn;
}

void TermPlayer::swap_turn()
{
    _is_its_turn = !_is_its_turn;
}
