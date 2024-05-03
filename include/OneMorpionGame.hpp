#pragma once

#include <array>
#include <memory>
#include "IPlayer.hpp"
#include "MorpionGame.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

class OneMorpionGame {
public:
    OneMorpionGame(std::array<player_ptr, 2> players);

    void         init();
    void         report_end();
    void         report_win();
    int          make_them_play();
    MorpionGame &get_game()
    {
        return _game;
    };

private:
    MorpionGame               _game;
    unsigned int              _current_player;
    std::array<player_ptr, 2> _players;
};
