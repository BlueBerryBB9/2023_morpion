#pragma once

#include "../include/OneMorpionGame.hpp"
#include <array>

OneMorpionGame::OneMorpionGame(std::array<player_ptr, 2> players)
    : _players{std::move(players)}
{}

void OneMorpionGame::init()
{
    _current_player = 0;
    _players[0]->set_board_state(_game.array());
    _players[1]->set_board_state(_game.array());
    if (_game.status() == MorpionGame::Status::PXTurn)
        _players[0]->set_turn(true);
    if (_game.status() == MorpionGame::Status::POTurn)
        _players[1]->set_turn(true);

    _players[!_current_player]->set_player_symbol();
    _players[_current_player]->ask_for_move();
}

int OneMorpionGame::make_them_play()
{
    if (_players[0]->is_done() || _players[1]->is_done()) {
        return 1;
    }

    _players[!_current_player]->process_events();
    _players[_current_player]->process_events();

    if (_players[_current_player]->get_move())
        if (_game.play(_players[_current_player]->get_sym(),
                       *_players[_current_player]->get_move())) {
            _players[0]->set_board_state(_game.array());
            _players[1]->set_board_state(_game.array());

            if (_players[0]->is_done() || _players[1]->is_done()) {
                return 1;
            }

            _players[0]->swap_turn();
            _players[1]->swap_turn();
            _current_player = !_current_player;

            _players[!_current_player]->set_player_symbol();
            _players[_current_player]->ask_for_move();
        }
    return 0;
}

void OneMorpionGame::report_end()
{}
