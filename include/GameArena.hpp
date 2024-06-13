#pragma once

#include <SFML/Network.hpp>
#include <array>
#include <memory>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

enum class PLAYER_REPLAY {
    NONE = 0,
    P1,
    P2,
};

class GameArena {
public:
    GameArena(player_ptr player1, player_ptr player2);
    GameArena(GameArena &&other);
    GameArena &operator=(GameArena &&other);

    void cycle_once();
    void run();
    bool done() const;

private:
    void _play();
    void _report_end();
    void _report_win();
    bool _players_or_game_done();
    bool _players_done();
    bool _game_done();
    void _set_done(bool done);
    bool _players_replay();
    void _reset();

    int                       _id;
    MorpionGame               _game;
    unsigned int              _current_player;
    bool                      _is_done{false};
    std::array<player_ptr, 2> _players;
    PLAYER_PHASE              _phase{PLAYER_PHASE::playing};
    PLAYER_REPLAY             _replay;
};
