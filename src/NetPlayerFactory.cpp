#include "NetPlayerFactory.hpp"
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <memory>
#include "StandaloneNetPlayer.hpp"

NetPlayerFactory::NetPlayerFactory(int port)
{
    _lstnr.setBlocking(false);
    if (_lstnr.listen(port) != sf::Socket::Done)
        throw std::runtime_error("ctor:listener:listen");
    _slctr.add(_lstnr);
    std::cout << "Server Started !" << std::endl;
}

std::unique_ptr<IPlayer> NetPlayerFactory::create_one()
{
    std::unique_ptr<sf::TcpSocket> sock{new sf::TcpSocket};
    _slctr.wait(sf::milliseconds(50));
    if (_slctr.isReady(_lstnr)) {
        if (_lstnr.accept(*sock) != sf::Socket::Done)
            throw std::runtime_error("createone:listener:accept");
        return std::unique_ptr<IPlayer>(new StandaloneNetPlayer(sock));
    }

    return nullptr;
}
