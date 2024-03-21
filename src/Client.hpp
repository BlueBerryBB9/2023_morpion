#pragma once

#include <array>
#include <functional>
#include <istream>
#include <unordered_map>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

class Client {
public:
    Client(IPlayer &player)
        : _board{'.', '.', '.', '.', '.', '.', '.', '.', '.'},
          _status{MorpionGame::Status::POTurn},
          _sym{'.'},
          _deser_map{
              {"board", [&](std::istream &is) { _deser_board(is); }},
              {"status", [&](std::istream &is) { _deser_status(is); }},
              {"your_sym", [&](std::istream &is) { _deser_sym(is); }},
              {"your_turn", [&](std::istream &) { _ask_for_move(); }},
          },
          _player(player),
          _done(false),
          _our_turn(false)
    {}

    std::istream &deserialise_line(std::istream &is);

    void cycle_once();

private:
    void _deser_board(std::istream &is);
    void _deser_status(std::istream &is);
    void _deser_sym(std::istream &is);
    void _ask_for_move();

    using DeserFunc = std::function<void(std::istream &)>;
    using DeserMap  = std::unordered_map<std::string, DeserFunc>;

    std::array<char, 9> _board;
    MorpionGame::Status _status;
    char                _sym;
    DeserMap            _deser_map;
    IPlayer            &_player;
    bool                _done;
    bool                _our_turn;
};
