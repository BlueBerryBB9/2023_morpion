#pragma once

#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <algorithm>
#include <any>
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
{}

void exec_function(sf::Packet &packet, GfxPlayer &player)
{
    std::string str;
    std::string str2;
    if (!(packet >> str))
        throw std::runtime_error("exec_function:packet_str");
    auto it{NO_ARGS_FUNCTIONS.find(str)};
    if (it != NO_ARGS_FUNCTIONS.end())
        it->second(player);
    else {
        if (str == "SET_BOARD_STATE"sv) {
            packet >> str2;
            player.set_board_state(convert_to_array(str2));
        }
    }

    void client_loop(sf::TcpSocket & sock, GfxPlayer & player)
    {
        sf::SocketSelector sect;
        sf::Packet         packet;

        sect.add(sock);
        while (1) {
            sect.wait();
            if (sect.isReady(sock)) {
                sock.receive(packet);
                exec_function(packet, player);
            }
        }
    }
