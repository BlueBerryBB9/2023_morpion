#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "../include/GameArena.hpp"
#include "../include/GfxPlayer.hpp"
#include "../include/IPlayer.hpp"
#include "../include/MorpionGame.hpp"
#include "../include/StandaloneNetPlayer.hpp"
#include "../include/TermPlayer.hpp"
#include "../include/client.hpp"

using namespace std::literals;

using player_ptr = std::unique_ptr<IPlayer>;

bool done(std::vector<std::unique_ptr<GameArena>> &g)
{
    for (auto it = g.begin(); it != g.end();) {
        if (it->get()->done())
            it = g.erase(it);
        else
            ++it;
    }
    return g.empty();
}

int main(int ac, char **av)
{
    try {
        if (ac < 2)
            throw std::runtime_error("main:not enough argument");
        if ("client"sv != av[1] && "host"sv != av[1] && "local"sv != av[1]
            && "server"sv != av[1])
            throw std::runtime_error("main:second argument wrong");
        if ("client"sv == av[1]) {
            Client cli;
            cli.client_loop();
        } else if ("local"sv == av[1]) {
            GameArena g{{player_ptr(new TermPlayer(MorpionGame::P1_CHAR)),
                         player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

            while (!g.done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.cycle_once();
            }
        } else if ("host"sv == av[1]) {
            GameArena g{
                {player_ptr(new StandaloneNetPlayer(MorpionGame::P1_CHAR)),
                 player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))}};

            while (!g.done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.cycle_once();
            }
        } else {
            int game_number;
            if (ac < 3 || !(std::stringstream(av[2]) >> game_number)
                || game_number < 1)
                throw std::runtime_error("main:third argument for server");

            std::vector<std::unique_ptr<GameArena>> g;

            for (int i = 0; i < game_number; i++)
                g.push_back(std::unique_ptr<GameArena>{new GameArena(
                    {player_ptr(new StandaloneNetPlayer(MorpionGame::P1_CHAR)),
                     player_ptr(
                         new StandaloneNetPlayer(MorpionGame::P2_CHAR))})});

            while (!done(g)) {
                for (auto it = g.begin(); it != g.end(); it++)
                    it->get()->cycle_once();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

    } catch (std::exception &e) {
        std::cout << "Game Stopped Suddenly :" << e.what() << std::endl;
    }
}
