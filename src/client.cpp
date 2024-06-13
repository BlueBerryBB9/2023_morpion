#include "client.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include "GfxPlayer.hpp"
#include "IPlayer.hpp"

Client::Client() : _last_clock(std::chrono::steady_clock::now())
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

    std::cout << "Please give an IPAdress to connect on :" << std::endl;
    std::cin >> str;

    std::cout << "Listening on ip : [" << str << "] and port : [" << res << "]"
              << std::endl;

    if (_sock.connect(str, res, sf::seconds(1)) != sf::Socket::Done)
        throw std::runtime_error("ctor:sock:connect");

    std::cout << "Socket connected" << std::endl;

    _player = player_ptr(new GfxPlayer('x'));
    std::cout << "created gfx" << std::endl;
}

Client::~Client()
{
    _sock.disconnect();
}

const std::array<char, 9> Client::_convert_str_to_array(std::string str1)
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

    if (packet.getDataSize() == 0)
        return false;

    if (!(packet >> str))
        throw std::runtime_error("exec_function:packet_str");

    auto it{NO_ARGS_FUNCTIONS.find(str)};

    if (it != NO_ARGS_FUNCTIONS.end()) {
        it->second(*_player);
        if (str == "SET_WIN"sv || str == "SET_DRAW"sv || str == "SET_LOSE"sv)
            return true;
    } else {
        if (str == "SET_BOARD_STATE"sv) {
            std::string str2;
            packet >> str2;
            _player->set_board_state(_convert_str_to_array(str2));
        } else if (str == "SET_TURN"sv) {
            bool res;
            packet >> res;
            _player->set_turn(res);
        } else if (str == "SET_SYM"sv) {
            int res;
            packet >> res;
            _player->set_sym((res ? 'o' : 'x'));
        } else if (str == "SET_PHASE"sv) {
            int          res;
            PLAYER_PHASE res2;
            packet >> res;
            res2 = static_cast<PLAYER_PHASE>(res);
            _player->set_phase(res2);
        }
    }
    return false;
}

sf::Socket::Status Client::_send_on_sock(std::string str)
{
    sf::Packet packet;
    packet << str;
    return _sock.send(packet);
}

sf::Socket::Status Client::_send_on_sock()
{
    sf::Packet packet;
    return _sock.send(packet);
}

bool Client::_is_sock_done()
{
    if (!_started)
        return false;

    std::chrono::time_point<std::chrono::steady_clock> now
        = std::chrono::steady_clock::now();

    if (now - _last_clock < std::chrono::seconds{2})
        return false;

    _last_clock = std::chrono::steady_clock::now();

    return (_send_on_sock() == sf::Socket::Disconnected);
}

void Client::client_loop()
{
    sf::Packet         packet;
    sf::SocketSelector sect;

    sect.add(_sock);
    while (!_player->is_done() && !_is_sock_done()) {
        sect.wait(sf::milliseconds(50));

        if (sect.isReady(_sock)) {
            _started = true;
            _sock.receive(packet);
            if (_parse_and_exec(packet)) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
            packet.clear();
        }

        _player->process_events();

        if (_player->get_move() != std::nullopt && !_played) {
            packet << _player->get_move().value();
            _sock.send(packet);
            packet.clear();
            _played = true;
        }
    }

    std::cout << "Game suddenly closed :"
              << (_player->is_done() ? "window closed"
                                     : "server closed connection")
              << std::endl;
}
