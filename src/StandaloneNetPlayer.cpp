#include "StandaloneNetPlayer.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

StandaloneNetPlayer::StandaloneNetPlayer(char sym)
    : _sym{sym},
      _sock{std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket())},
      _last_clock{std::chrono::steady_clock::now()}
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

    if (lstnr.accept(*_sock) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:accept");

    std::cout << "Socket accepted !" << std::endl;

    lstnr.close();
    _sect.add(*_sock);

    sf::Packet packet;
    packet << std::string("SET_SYM") << (_sym == 'x' ? 0 : 1);
    _send_on_sock(packet);
}

StandaloneNetPlayer::StandaloneNetPlayer(std::unique_ptr<sf::TcpSocket> &sock)
    : _sock(std::move(sock)), _last_clock{std::chrono::steady_clock::now()}
{
    _sect.add(*_sock);
}

StandaloneNetPlayer::~StandaloneNetPlayer()
{
    _sock->disconnect();
}

sf::Socket::Status StandaloneNetPlayer::_send_on_sock(std::string str)
{
    sf::Packet packet;
    packet << str;
    return _sock->send(packet);
}

sf::Socket::Status StandaloneNetPlayer::_send_on_sock(sf::Packet &packet)
{
    return _sock->send(packet);
}

sf::Socket::Status StandaloneNetPlayer::_send_on_sock()
{
    sf::Packet packet;
    return _sock->send(packet);
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
    std::string str2;
    sf::Packet  packet;

    for (int i = 0; i < static_cast<int>(board.size()); i++)
        str2 = str2 + board[i];

    packet << str << str2;

    _send_on_sock(packet);
}

bool StandaloneNetPlayer::is_done()
{
    return _done;
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
    sf::Packet  packet;
    std::string str("SET_TURN");

    _is_its_turn = your_turn;

    packet << str;
    packet << your_turn;
    _send_on_sock(packet);
}

void StandaloneNetPlayer::swap_turn()
{
    _is_its_turn = !_is_its_turn;
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

std::optional<int> StandaloneNetPlayer::_receive_on_sock()
{
    int         res;
    sf::Packet  packet;
    std::string func;

    if (_sock->receive(packet) == sf::Socket::Disconnected) {
        _done = true;
        return {};
    }

    if (packet.getDataSize() == 0)
        return {};

    packet >> res;

    return res;
}

void StandaloneNetPlayer::process_events()
{
    _move_made.reset();

    if (_done || _connection_closed()) {
        _done = true;
        return;
    }

    if (!_is_its_turn)
        return;

    if (_can_ask_again)
        ask_for_move();

    _sect.wait(sf::milliseconds(50));
    if (_sect.isReady(*_sock)) {
        _move_made = _receive_on_sock();
        if (_move_made)
            _can_ask_again = true;
    }
}

bool StandaloneNetPlayer::_connection_closed()
{
    std::chrono::time_point<std::chrono::steady_clock> now
        = std::chrono::steady_clock::now();

    if (now - _last_clock < std::chrono::seconds{2})
        return false;

    _last_clock = std::chrono::steady_clock::now();

    return (_send_on_sock() == sf::Socket::Disconnected);
}

void StandaloneNetPlayer::set_sym(char sym)
{
    _sym = sym;
    sf::Packet packet;
    packet << std::string("SET_SYM") << (_sym == 'x' ? 0 : 1);
    _send_on_sock(packet);
}

void StandaloneNetPlayer::wait()
{
    sf::Packet packet;
    packet << std::string("WAIT");
    _send_on_sock(packet);
}

void StandaloneNetPlayer::play_again()
{
    sf::Packet packet;
    packet << std::string("PLAY_AGAIN");
    _send_on_sock(packet);
}

void StandaloneNetPlayer::replay()
{
    sf::Packet packet;
    packet << std::string("REPLAY");
    _send_on_sock(packet);
}
