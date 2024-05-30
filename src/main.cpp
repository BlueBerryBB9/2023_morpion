#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <unistd.h>
#include "../include/GfxPlayer.hpp"
#include "../include/IPlayer.hpp"
#include "../include/MorpionGame.hpp"
#include "../include/OneMorpionGame.hpp"
#include "../include/StandaloneNetPlayer.hpp"
#include "../include/TermPlayer.hpp"
#include "../include/client.hpp"

using namespace std::literals;

using player_ptr = std::unique_ptr<IPlayer>;

int main(int ac, char **av)
{
    try {
        if (ac < 2)
            throw std::runtime_error("main:argument number");
        if ("client"sv != av[1] && "host"sv != av[1] && "local"sv != av[1])
            throw std::runtime_error("main:second argument");
        if ("client"sv == av[1]) {
            Client cli;
            cli.client_loop();
        } else if ("local"sv == av[1]) {
            OneMorpionGame g{{player_ptr(new TermPlayer(MorpionGame::P1_CHAR)),
                              player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

            while (!g.is_done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.run_once();
            }
        } else {
            OneMorpionGame g{
                {player_ptr(new StandaloneNetPlayer(MorpionGame::P1_CHAR)),
                 player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

            while (!g.is_done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.run_once();
            }
        }
    } catch (std::exception &e) {
        std::cout << "Game Stopped Suddenly :" << e.what() << std::endl;
    }
}
