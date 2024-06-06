#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include "GameArena.hpp"
#include "GfxPlayer.hpp"
#include "IPlayer.hpp"
#include "MorpionGame.hpp"
#include "NetPlayerFactory.hpp"
#include "StandaloneNetPlayer.hpp"
#include "TermPlayer.hpp"
#include "client.hpp"

using namespace std::literals;

using player_ptr = std::unique_ptr<IPlayer>;

void done(std::vector<GameArena> &g)
{
    for (auto it = g.begin(); it != g.end();) {
        if (it->done())
            it = g.erase(it);
        else
            ++it;
    }
}

void run_server()
{
    std::vector<GameArena>                arenas;
    std::vector<std::unique_ptr<IPlayer>> _players;
    NetPlayerFactory                      factory{1234};

    while (1) {
        done(arenas);
        std::for_each(arenas.begin(), arenas.end(),
                      [](GameArena &arena) { arena.cycle_once(); });
        auto new_player = factory.create_one();
        if (new_player) {
            _players.push_back(std::move(new_player));
            if (_players.size() % 2 == 0) {
                int last_idx = _players.size() - 1;
                arenas.emplace_back(std::move(_players[last_idx - 1]),
                                    std::move(_players[last_idx]));
                _players.pop_back();
                _players.pop_back();
            } else {
                _players.back()->wait();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
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
            GameArena g{player_ptr(new TermPlayer(MorpionGame::P1_CHAR)),
                        player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))};

            while (!g.done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.cycle_once();
            }
        } else if ("host"sv == av[1]) {
            GameArena g{
                player_ptr(new StandaloneNetPlayer(MorpionGame::P1_CHAR)),
                player_ptr(new GfxPlayer(MorpionGame::P2_CHAR))};

            while (!g.done()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                g.cycle_once();
            }
        } else {
            run_server();
        }

    } catch (std::exception &e) {
        std::cout << "Game Stopped Suddenly :" << e.what() << std::endl;
    }
}
