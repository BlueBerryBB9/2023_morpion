#include "../include/StandaloneNetPlayer.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

StandaloneNetPlayer::StandaloneNetPlayer(char sym) : _sym{sym}
{
    int             res;
    sf::TcpListener lstnr;

    std::cout << "Please give a port to listen on :" << std::endl;
    while (!(std::cin >> res))
        std::cout << "Please give a port to listen on :" << std::endl;

    if (lstnr.listen(res) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:listen");

    if (lstnr.accept(_sock) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:accept");

    std::cout << "Socket accepted !" << std::endl;

    lstnr.close();
    _sect.add(_sock);
    std::string mess = "SET_SYM ";
    mess += _sym;
    _send_on_sock(mess);
}

StandaloneNetPlayer::~StandaloneNetPlayer()
{
    if (_sock.getRemotePort() != 0)
        _sock.disconnect();
}

void StandaloneNetPlayer::_send_on_sock(std::string str)
{
    _sock.send(str.c_str(), str.size());
}

std::string StandaloneNetPlayer::_receive_on_sock()
{
    // NEED TO MODIFY THIS SO I CAN USE PACKET
    // char *str;
    // // std::size_t bytes_read;
    //
    // str = new char[30];
    // while (_sock.receive()) {
    //
    // }
    // str[bytes_read] = '\0';

    return std::string("nothing for now");
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
    std::string str;

    for (int i = 0; i < static_cast<int>(board.size()); i++)
        str = str + board[i];

    _send_on_sock("SET_BOARD_STATE" + str);
}

bool StandaloneNetPlayer::is_done()
{
    return (_sock.getRemotePort() == 0);
}

void StandaloneNetPlayer::ask_for_move()
{
    _send_on_sock("ASK_FOR_MOVE");
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
    _sect.wait();
    if (_sect.isReady(_sock)) {
        std::string res(_receive_on_sock());
    }
}
