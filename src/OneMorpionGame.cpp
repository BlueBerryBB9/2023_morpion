#include "../include/OneMorpionGame.hpp"
#include <array>
#include <functional>
#include <iostream>

using func_on_2_players = std::function<void(IPlayer &, IPlayer &)>;

const std::unordered_map<MorpionGame::Status, func_on_2_players> STATUS_MAP = {
    {MorpionGame::Status::Draw,
     [](IPlayer &x, IPlayer &o) {
         x.set_draw();
         o.set_draw();
     }},
    {MorpionGame::Status::PXWin,
     [](IPlayer &x, IPlayer &o) {
         x.set_win();
         o.set_win();
     }},
    {MorpionGame::Status::POWin,
     [](IPlayer &x, IPlayer &o) {
         x.set_win();
         o.set_win();
     }},
};

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
{
    if (_players[0]->is_done() && _players[1]->is_done()) {
        std::cout << "Error : All _players exited";
    } else if (_players[0]->is_done() && !_players[1]->is_done()) {
        std::cout << "Error : Player " << MorpionGame::P1_CHAR << " exited !"
                  << std::endl;
    } else if (!_players[0]->is_done() && _players[1]->is_done()) {
        std::cout << "Error : Player " << MorpionGame::P2_CHAR << " exited !"
                  << std::endl;
    }
}

void OneMorpionGame::report_win()
{
    auto status{_game.status()};
    auto found_iter{STATUS_MAP.find(status)};

    if (found_iter != STATUS_MAP.end()) {
        found_iter->second(*_players[0], *_players[1]);
    } else {
        std::cerr << "the game hasn't ended properly\n";
    }
}
