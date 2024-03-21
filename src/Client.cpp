#include "Client.hpp"
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

void Client::_deser_board(std::istream &is)
{
    std::string tmp;
    auto        board_iter{_board.begin()};
    auto        str_iter{tmp.cbegin()};

    if (is >> tmp) {
        while (str_iter != tmp.cend() && board_iter != _board.end()) {
            *board_iter = *str_iter;
            board_iter++;
            str_iter++;
        }
    }
    _player.set_board_state(_board);
}

void Client::_deser_status(std::istream &is)
{
    int tmp;

    is >> tmp;
    _status = static_cast<MorpionGame::Status>(tmp);
}

void Client::_deser_sym(std::istream &is)
{
    char c;

    is >> c;
   _player.set_player_symbol(_sym);
}

void Client::_ask_for_move(void)
{
    _our_turn = true;
    _player.ask_for_move(_sym);
}

std::istream &Client::deserialise_line(std::istream &is)
{
    std::string verb;
    is >> verb;
    auto iter{_deser_map.find(verb)};
    if (iter != _deser_map.end())
        iter->second(is);
    return is;
}

void Client::cycle_once(void)
{
    if (_done || _player.get_status() == IPlayer::Status::Left)
        return;
    if (_our_turn == true) {
        _player.on_cycle();
        if (_player.get_status() == IPlayer::Status::MoveMade) {
            auto move = _player.get_move();
            _our_turn = false;
            if (move.has_value()) {
                // TODO: send the move
            }
        }
        if (_player.get_status() == IPlayer::Status::Left) {
            // TODO: send message quit
        }
    }
}
