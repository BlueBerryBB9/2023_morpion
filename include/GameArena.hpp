#pragma once

#include <SFML/Network.hpp>
#include <array>
#include <memory>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

class GameArena {
public:
    GameArena(player_ptr player1, player_ptr player2);
    GameArena(GameArena &&other);
    GameArena &operator=(GameArena &&other);

    void cycle_once();
    void run();
    bool done() const;

private:
    bool _players_or_game_done();
    void _report_end();

    int                       _id;
    MorpionGame               _game;
    unsigned int              _current_player;
    bool                      _is_done{false};
    std::array<player_ptr, 2> _players;
};
