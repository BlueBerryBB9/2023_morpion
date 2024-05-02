#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include "../include/GfxPlayer.hpp"
#include "../include/IPlayer.hpp"
#include "../include/MorpionGame.hpp"
#include "../include/OneMorpionGame.hpp"
#include "../include/TermPlayer.hpp"

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

void report_win(MorpionGame &game, IPlayer &x, IPlayer &o)
{
    auto status{game.status()};
    auto found_iter{STATUS_MAP.find(status)};

    if (found_iter != STATUS_MAP.end()) {
        found_iter->second(x, o);
    } else {
        std::cerr << "the game hasn't ended properly\n";
    }
}

using player_ptr = std::unique_ptr<IPlayer>;

void report_end(std::array<player_ptr, 2> &players)
{
    if (players[0]->is_done() && players[1]->is_done()) {
        std::cout << "Error : All players exited";
    } else if (players[0]->is_done() && !players[1]->is_done()) {
        std::cout << "Error : Player " << MorpionGame::P1_CHAR << " exited !"
                  << std::endl;
    } else if (!players[0]->is_done() && players[1]->is_done()) {
        std::cout << "Error : Player " << MorpionGame::P2_CHAR << " exited !"
                  << std::endl;
    }
}

int main(void)
{
    OneMorpionGame g{{player_ptr(new TermPlayer(MorpionGame::P1_CHAR)),
                      player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

    g.init();

    while (!game.done()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // if (g.make_them_play(game, players, current_player)) {
        if (g.make_them_play()) {
            report_end(players);
            return 1;
        }
    }
    report_win(game, *players[0], *players[1]);
}
