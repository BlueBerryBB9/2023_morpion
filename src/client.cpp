#include "../include/client.hpp"
#include <SFML/Network.hpp>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include "../include/GfxPlayer.hpp"

const std::array<char, 9> Client::_convert_to_array(std::string str1)
{
    int                 i;
    std::array<char, 9> arr = {};

    for (i = 0; i < 9; i++)
        arr[i] = str1[i];

    return arr;
}

bool Client::_parse_and_exec(sf::Packet &packet)
{
    std::string str;
    if (!(packet >> str))

        throw std::runtime_error("exec_function:packet_str");

    if (!str.empty())
        std::cout << "STR : " << str << std::endl;

    auto it{NO_ARGS_FUNCTIONS.find(str)};

    if (it != NO_ARGS_FUNCTIONS.end()) {
        it->second(*_player);
        if (str == "ASK_FOR_MOVE"sv)
            _played = false;
        if (str == "SET_WIN"sv || str == "SET_DRAW"sv || str == "SET_LOSE"sv)
            return true;
    } else {
        if (str == "SET_BOARD_STATE"sv) {
            std::string str2;
            packet >> str2;
            _player->set_board_state(_convert_to_array(str2));
        } else if (str == "SET_TURN"sv) {
            bool res;
            packet >> res;
            _player->set_turn(res);
        }
    }
    return false;
}

bool Client::_is_sock_done()
{
    bool       res;
    sf::Packet packet;

    packet << std::string("");

    _sock.setBlocking(false);
    res = (_sock.send(packet) == sf::Socket::Disconnected);
    _sock.setBlocking(true);

    return res;
}

void Client::client_loop()
{
    sf::Packet         packet;
    sf::SocketSelector sect;

    sect.add(_sock);
    while (!_player->is_done() && !_is_sock_done()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        sect.wait();
        if (sect.isReady(_sock)) {
            _sock.receive(packet);
            if (_parse_and_exec(packet))
                return;
            packet.clear();
        }

        _player->process_events();

        if (_player->get_move() != std::nullopt && !_played) {
            std::cout << "MOVE IS DONE" << std::endl;
            packet << std::string("MOVE") << _player->get_move().value();
            _sock.send(packet);
            packet.clear();
            _played = true;
        }
    }
}

Client::Client()
{
    int         res;
    std::string str;

    std::cout << "Initializing client" << std::endl;

    std::cout << "Please give a port to connect on :" << std::endl;

    while (!(std::cin >> res)) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << "Please give a port to connect on :" << std::endl;
    }

    if (_sock.connect(sf::IpAddress("localhost"), res) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:listen");

    std::cout << "Socket connected" << std::endl;

    sf::Packet packet;
    _sock.receive(packet);
    packet >> str >> res;

    _player = player_ptr(new GfxPlayer((res ? 'o' : 'x')));
}
