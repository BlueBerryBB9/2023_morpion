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
    bool _players_done();
    bool _game_done();
    void _report_end();
    void _report_win();
    void _set_done(bool done);

    int                       _id;
    MorpionGame               _game;
    unsigned int              _current_player;
    bool                      _is_done{false};
    bool                      _replay_mode{false};
    std::array<player_ptr, 2> _players;
};
