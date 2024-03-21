#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include "GfxPlayer.hpp"
#include "IPlayer.hpp"
#include "MorpionGame.hpp"
#include "TermPlayer.hpp"

using func_on_2_players = std::function<void(IPlayer &, IPlayer &)>;

const std::unordered_map<MorpionGame::Status, func_on_2_players> STATUS_MAP = {
    {MorpionGame::Status::Draw,
     [](IPlayer &x, IPlayer &o) {
         x.set_draw();
         o.set_draw();
     }},
    {MorpionGame::Status::PXWin,
     [](IPlayer &x, IPlayer &o) {
         x.set_win('x');
         o.set_win('x');
     }},
    {MorpionGame::Status::POWin,
     [](IPlayer &x, IPlayer &o) {
         x.set_win('o');
         o.set_win('o');
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

void make_them_play(MorpionGame &game, IPlayer &player, char sym)
{
    bool played{false};

    player.set_board_state(game.array());
    while (!played) {
        if (player.is_done())
            return;
        auto move{player.get_move(sym)};
        if (move) {
            played = game.play(sym, *move);
        }
    }
    player.set_board_state(game.array());
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
    MorpionGame               game;
    std::array<player_ptr, 2> players{player_ptr(new GfxPlayer),
                                      player_ptr(new GfxPlayer)};
    unsigned int              current_player{0};

    players[0]->set_player_symbol('x');
    players[0]->set_board_state(game.array());
    players[1]->set_player_symbol('o');
    players[1]->set_board_state(game.array());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (!game.done()) {
        if (players[0]->is_done() || players[1]->is_done()) {
            report_end(players);
            return 1;
        }
        make_them_play(game, *players[current_player],
                       (current_player) ? 'o' : 'x');
        current_player = !current_player;
    }
    report_win(game, *players[0], *players[1]);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
