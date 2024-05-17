#include "../include/StandaloneNetPlayer.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

StandaloneNetPlayer::StandaloneNetPlayer(char sym) : _sym{sym}
{
    int             res;
    std::string     str;
    sf::TcpListener lstnr;

    std::cout << "NetPlayer Initialization" << std::endl;

    std::cout << "Please give a port to listen on :" << std::endl;

    while (!(std::cin >> res)) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << "Please give a port to listen on :" << std::endl;
    }

    if (lstnr.listen(res) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:listen");

    std::cout << "Listening..." << std::endl;

    if (lstnr.accept(_sock) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:accept");

    std::cout << "Socket accepted !" << std::endl;

    lstnr.close();
    _sect.add(_sock);

    sf::Packet packet;
    packet << std::string("SET_SYM") << (_sym == 'x' ? 0 : 1);
    _send_on_sock(packet);
}

StandaloneNetPlayer::~StandaloneNetPlayer()
{
    if (_sock.getRemotePort() != 0)
        _sock.disconnect();
}

sf::Socket::Status StandaloneNetPlayer::_send_on_sock(std::string str)
{
    sf::Packet packet;
    packet << str;
    return _sock.send(packet);
}

sf::Socket::Status StandaloneNetPlayer::_send_on_sock(sf::Packet packet)
{
    return _sock.send(packet);
}

int StandaloneNetPlayer::_receive_on_sock()
{
    int         res;
    sf::Packet  packet;
    std::string func;

    _sock.receive(packet);

    packet >> func;

    if (func != "MOVE")
        throw std::runtime_error("receive on sock: MOVE string not found");

    packet >> res;

    return res;
}

void StandaloneNetPlayer::set_win()
{
    _send_on_sock("SET_WIN");
}

void StandaloneNetPlayer::set_draw()
{
    _send_on_sock("SET_DRAW");
}

void StandaloneNetPlayer::set_lose()
{
    _send_on_sock("SET_LOSE");
}

void StandaloneNetPlayer::set_board_state(const std::array<char, 9> &board)
{
    std::string str("SET_BOARD_STATE");
    sf::Packet  packet;

    packet << str;
    str.clear();

    for (int i = 0; i < static_cast<int>(board.size()); i++)
        str = str + board[i];
    packet << str;

    _send_on_sock(packet);
}

bool StandaloneNetPlayer::is_done()
{
    _sock.setBlocking(false);
    return (_send_on_sock("") == sf::Socket::Disconnected);
}

void StandaloneNetPlayer::ask_for_move()
{
    if (_can_ask_again) {
        _send_on_sock("ASK_FOR_MOVE");
        _can_ask_again = false;
    }
}

void StandaloneNetPlayer::set_turn(bool your_turn)
{
    std::string str((your_turn ? "1" : "0"));
    _send_on_sock("SET_TURN " + str);
}

void StandaloneNetPlayer::swap_turn()
{
    _send_on_sock("SWAP_TURN");
}

void StandaloneNetPlayer::set_player_symbol()
{
    _send_on_sock("SET_PLAYER_SYMBOL");
}

char StandaloneNetPlayer::get_sym()
{
    return _sym;
}

std::optional<unsigned int> StandaloneNetPlayer::get_move()
{
    return _move_made ? _move_made : std::nullopt;
}

void StandaloneNetPlayer::process_events()
{
    _move_made.reset();

    if (!_is_its_turn)
        return;
    if (_can_ask_again)
        ask_for_move();
    _sect.wait();
    if (_sect.isReady(_sock)) {
        _move_made     = _receive_on_sock();
        _can_ask_again = true;
    }
}
