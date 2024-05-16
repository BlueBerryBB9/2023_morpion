#pragma once

#include <array>
#include <memory>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

class OneMorpionGame {
public:
    OneMorpionGame(std::array<player_ptr, 2> players);

    void report_end();
    void run_once();

    bool is_done()
    {
        return _is_done;
    }

    bool players_or_game_done()
    {
        return (_players[0]->is_done() || _players[1]->is_done()
                || _game.done());
    }

private:
    MorpionGame               _game;
    unsigned int              _current_player;
    std::array<player_ptr, 2> _players;
    bool                      _is_done{false};
};
