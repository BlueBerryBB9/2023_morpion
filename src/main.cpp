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
#include "./client.cpp"

using namespace std::literals;

using player_ptr = std::unique_ptr<IPlayer>;

void client()
{
    int           res;
    std::string   str;
    sf::TcpSocket sock;

    std::cout << "Initializing client" << std::endl;

    std::cout << "Please give a port to connect on :" << std::endl;

    while (!(std::cin >> res)) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << "Please give a port to connect on :" << std::endl;
    }

    if (sock.connect(sf::IpAddress("localhost"), res) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:listen");

    std::cout << "Socket connected" << std::endl;

    sf::Packet packet;
    sock.receive(packet);
    packet >> str;
    packet >> res;

    std::cout << "str : " + str << std::endl;
    std::cout << "res : " << res << std::endl;

    GfxPlayer player((res ? 'o' : 'x'));

    client_loop(sock, player);
    sock.disconnect();
}

int main(int ac, char **av)
{
    try {
        if (ac < 2)
            throw std::runtime_error("main:argument number");
        if ("client"sv != av[1] && "host"sv != av[1] && "local"sv != av[1])
            throw std::runtime_error("main:second argument");
        if ("client"sv == av[1]) {
            client();
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
