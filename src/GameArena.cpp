#include "GameArena.hpp"
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>
#include "MorpionGame.hpp"

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
         o.set_lose();
     }},
    {MorpionGame::Status::POWin,
     [](IPlayer &x, IPlayer &o) {
         x.set_lose();
         o.set_win();
     }},
};

GameArena::GameArena(player_ptr player1, player_ptr player2)
    : _players{{std::move(player1), std::move(player2)}}
{
    static int created = 0;
    created++;
    _id = created;
    std::cout << "GameArena " << _id << " launched" << std::endl;

    _current_player = (_game.status() == MorpionGame::Status::PXTurn ? 0 : 1);

    _players[0]->set_sym(MorpionGame::P1_CHAR);
    _players[1]->set_sym(MorpionGame::P2_CHAR);

    _players[0]->set_board_state(_game.array());
    _players[1]->set_board_state(_game.array());

    if (_game.status() == MorpionGame::Status::PXTurn)
        _players[0]->set_turn(true);
    if (_game.status() == MorpionGame::Status::POTurn)
        _players[1]->set_turn(true);

    _players[!_current_player]->set_player_symbol();
    _players[_current_player]->ask_for_move();
}

GameArena::GameArena(GameArena &&other)
    : _players{std::move(other._players[0]), std::move(other._players[1])}
{}

GameArena &GameArena::operator=(GameArena &&other)
{
    this->_players[0]     = std::move(other._players[0]);
    this->_players[1]     = std::move(other._players[1]);
    this->_game           = other._game;
    this->_current_player = other._current_player;
    this->_id             = other._id;
    this->_is_done        = other._is_done;
    return *this;
}

void GameArena::cycle_once()
{
    if (done())
        return;

    if (_replay_mode) {
    }
    if (_players_done())
        return _report_end();

    _players[!_current_player]->process_events();
    _players[_current_player]->process_events();

    if (_players[_current_player]->get_move())
        if (_game.play(_players[_current_player]->get_sym(),
                       *_players[_current_player]->get_move())) {
            _players[0]->set_board_state(_game.array());
            _players[1]->set_board_state(_game.array());

            if (_game_done()) {
                _report_win();
                _players[0]->replay();
                _players[1]->replay();
                _replay_mode = true;
                return;
            }

            if (_players_done())
                return _report_end();

            _players[0]->swap_turn();
            _players[1]->swap_turn();
            _current_player = !_current_player;

            _players[!_current_player]->set_player_symbol();
            _players[_current_player]->ask_for_move();
        }
}

void GameArena::run()
{
    while (!_players_or_game_done()) {
        _players[!_current_player]->process_events();
        _players[_current_player]->process_events();

        if (_players[_current_player]->get_move())
            if (_game.play(_players[_current_player]->get_sym(),
                           *_players[_current_player]->get_move())) {
                _players[0]->set_board_state(_game.array());
                _players[1]->set_board_state(_game.array());

                if (_game_done()) {
                    _report_win();
                    _players[0]->replay();
                    _players[1]->replay();
                    _replay_mode = true;
                    return;
                }

                if (_players_done())
                    return _report_end();

                _players[0]->swap_turn();
                _players[1]->swap_turn();
                _current_player = !_current_player;

                _players[!_current_player]->set_player_symbol();
                _players[_current_player]->ask_for_move();
            }
    }
    return _report_end();
}

void GameArena::_report_end()
{
    _is_done = true;

    if (_players[0]->is_done() || _players[1]->is_done()) {
        if (_players[0]->is_done() && !_players[1]->is_done()) {
            std::cout << "Error : Player " << MorpionGame::P1_CHAR
                      << " exited !" << std::endl;
        } else if (!_players[0]->is_done() && _players[1]->is_done()) {
            std::cout << "Error : Player " << MorpionGame::P2_CHAR
                      << " exited !" << std::endl;
        } else {
            std::cout << "Error : All _players exited";
        }
        std::cout << "GameArena " << _id << " finished " << std::endl;
        return;
    }
}

void GameArena::_report_win()
{
    auto status{_game.status()};
    auto found_iter{STATUS_MAP.find(status)};

    if (found_iter != STATUS_MAP.end())
        found_iter->second(*_players[0], *_players[1]);
    else
        std::cerr << "the game hasn't ended properly" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

bool GameArena::done() const
{
    return _is_done;
}

bool GameArena::_players_or_game_done()
{
    return (_players[0]->is_done() || _players[1]->is_done() || _game.done());
}

bool GameArena::_players_done()
{
    return (_players[0]->is_done() || _players[1]->is_done());
}

bool GameArena::_game_done()
{
    return _game.done();
}

void GameArena::_set_done(bool done)
{
    if (done)
        std::cout << "GameArena " << _id << " finished " << std::endl;

    _is_done = true;
}
