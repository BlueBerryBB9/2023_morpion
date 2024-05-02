#include <array>
#include <chrono>
#include <cstdio>
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

int make_them_play(MorpionGame &game, std::array<player_ptr, 2> &players,
                   int current_player)
{
    bool played{false};

    if (players[0]->is_done() || players[1]->is_done()) {
        report_end(players);
        return 1;
    }

    players[!current_player]->set_player_symbol(current_player ? 'x' : 'o');
    players[current_player]->ask_for_move(current_player ? 'o' : 'x');

    while (!played) {
        players[current_player]->ask_for_move(current_player ? 'o' : 'x');
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (players[0]->is_done() || players[1]->is_done()) {
            return 1;
        }

        players[!current_player]->get_move(current_player ? 'o' : 'x');

        auto move{
            players[current_player]->get_move(current_player ? 'o' : 'x')};

        if (move)
            played = game.play(current_player ? 'o' : 'x', *move);
    }

    players[0]->set_board_state(game.array());
    players[1]->set_board_state(game.array());

    return 0;
}

int main(void)
{
    MorpionGame               game;
    std::array<player_ptr, 2> players{player_ptr(new TermPlayer),
                                      player_ptr(new GfxPlayer)};
    unsigned int              current_player{0};

    players[0]->set_player_symbol('x');
    players[0]->set_board_state(game.array());
    players[0]->set_turn(true);
    players[1]->set_player_symbol('o');
    players[1]->set_board_state(game.array());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    while (!game.done()) {
        if (make_them_play(game, players, current_player)) {
            report_end(players);
            return 1;
        }
        players[0]->swap_turn();
        players[1]->swap_turn();
        current_player = !current_player;
    }
    report_win(game, *players[0], *players[1]);
}
