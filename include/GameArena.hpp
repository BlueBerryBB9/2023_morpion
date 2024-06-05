#pragma once

#include <SFML/Network.hpp>
#include <array>
#include <memory>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

class GameArena {
public:
    GameArena(std::array<player_ptr, 2> players);

    void cycle_once();
    void run();
    bool done() const;

private:
    bool _players_or_game_done();
    void _report_end();

    MorpionGame               _game;
    unsigned int              _current_player;
    std::array<player_ptr, 2> _players;
    bool                      _is_done{false};
    sf::TcpListener           _lsntr;
};
