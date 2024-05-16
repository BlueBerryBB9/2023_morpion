#include "../include/StandaloneNetPlayer.hpp"
#include <SFML/Network/TcpListener.hpp>
#include <iostream>
#include <stdexcept>

StandaloneNetPlayer::StandaloneNetPlayer(char sym) : _sym{sym}
{
    int res;
    std::cout << "Please give a port to listen on :" << std::endl;
    while (!(std::cin >> res))
        std::cout << "Please give a port to listen on :" << std::endl;
    if (_lstnr.listen(res) != sf::Socket::Done)
        throw std::runtime_error("ctor:listen");
    _sect.add(_lstnr);
}

char StandaloneNetPlayer::get_sym()
{
    return _sym;
}
