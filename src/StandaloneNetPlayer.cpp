#include "../include/StandaloneNetPlayer.hpp"
#include <SFML/Network/TcpListener.hpp>
#include <iostream>

StandaloneNetPlayer::StandaloneNetPlayer(char sym) : _sym{sym}
{
    int res;
    std::cout << "Please give a port to listen on :" << std::endl;
    while (!(std::cin >> res))
        std::cout << "Please give a port to listen on :" << std::endl;
    while (_lstnr.listen(res)) {
    }
}

char StandaloneNetPlayer::get_sym()
{
    return _sym;
}
