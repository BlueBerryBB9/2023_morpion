#include <chrono>
#include <memory>
#include <thread>
#include <unistd.h>
#include "../include/GfxPlayer.hpp"
#include "../include/IPlayer.hpp"
#include "../include/MorpionGame.hpp"
#include "../include/OneMorpionGame.hpp"
#include "../include/TermPlayer.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

int main(void)
{
    OneMorpionGame g{{player_ptr(new TermPlayer(MorpionGame::P1_CHAR)),
                      player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

    g.init();

    while (!g.get_game().done()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // if (g.make_them_play(game, players, current_player)) {
        if (g.make_them_play()) {
            g.report_end();
            return 1;
        }
    }
    g.report_win();
}
