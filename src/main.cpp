#include <chrono>
#include <memory>
#include <thread>
#include <unistd.h>
#include "../include/GfxPlayer.hpp"
#include "../include/IPlayer.hpp"
#include "../include/MorpionGame.hpp"
#include "../include/OneMorpionGame.hpp"
#include "../include/StandaloneNetPlayer.hpp"
#include "../include/TermPlayer.hpp"

using player_ptr = std::unique_ptr<IPlayer>;

int main(void)
{
    OneMorpionGame g{{player_ptr(new StandaloneNetPlayer(MorpionGame::P1_CHAR)),
                      player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

    while (!g.is_done()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        g.run_once();
    }
}
