#pragma once

#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <array>
#include <functional>
#include <stdexcept>
#include "../include/GfxPlayer.hpp"

using namespace std::literals;

using func_player = std::function<void(IPlayer &)>;

const std::unordered_map<std::string, func_player> NO_ARGS_FUNCTIONS = {
    {"SET_WIN", [](IPlayer &x) { x.set_win(); }},
    {"SET_DRAW", [](IPlayer &x) { x.set_draw(); }},
    {"SET_LOSE", [](IPlayer &x) { x.set_lose(); }},
    {"SET_PLAYER_SYMBOL", [](IPlayer &x) { x.set_player_symbol(); }},
    {"ASK_FOR_MOVE", [](IPlayer &x) { x.ask_for_move(); }},
    {"SWAP_TURN", [](IPlayer &x) { x.swap_turn(); }},
};

const std::array<char, 9> convert_to_array(std::string str)
{
    int                 i{0};
    std::array<char, 9> arr = {};

    for (auto c : str)
        arr[i] = c;

    return arr;
}

bool parse_and_exec(sf::Packet &packet, GfxPlayer &player)
{
    std::string str;
    if (!(packet >> str))
        throw std::runtime_error("exec_function:packet_str");
    std::cout << "STR : " << str << std::endl;
    auto it{NO_ARGS_FUNCTIONS.find(str)};
    if (it != NO_ARGS_FUNCTIONS.end()) {
        it->second(player);
        if (str == "SET_WIN"sv || str == "SET_DRAW"sv || str == "SET_LOSE"sv)
            return true;
    } else {
        if (str == "SET_BOARD_STATE"sv) {
            std::string str2;
            packet >> str2;
            std::cout << "STR2 : " << str << std::endl;
            player.set_board_state(convert_to_array(str2));
        } else if (str == "SET_TURN"sv) {
            bool res;
            packet >> res;
            std::cout << "RES : " << res << std::endl;
            player.set_turn(res);
        }
    }
    return false;
}

bool is_sock_done(sf::TcpSocket &sock)
{
    bool res;
    sock.setBlocking(false);
    sf::Packet packet;
    packet << std::string("");
    res = (sock.send(packet) == sf::Socket::Disconnected);
    sock.setBlocking(true);
    return res;
}

void client_loop(sf::TcpSocket &sock, GfxPlayer &player)
{
    sf::SocketSelector sect;
    sf::Packet         packet;

    sect.add(sock);
    while (!player.is_done() && !is_sock_done(sock)) {
        sect.wait();
        if (sect.isReady(sock)) {
            sock.receive(packet);
            if (parse_and_exec(packet, player))
                return;
            packet.clear();
        }
        player.process_events();
        if (player.get_move()) {
            packet << std::string("MOVE");
            packet << player.get_move().value();
            sock.send(packet);
            packet.clear();
        }
    }
}
