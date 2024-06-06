#ifndef NET_PLAYER_FACTORY_HPP
#define NET_PLAYER_FACTORY_HPP

#include <SFML/Network.hpp>
#include <memory>
#include "IPlayer.hpp"

class NetPlayerFactory {
public:
    NetPlayerFactory(int port);

    std::unique_ptr<IPlayer> create_one(char sym);

private:
    sf::TcpListener    _lstnr;
    sf::SocketSelector _slctr;
};

#endif
