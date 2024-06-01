#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Network.hpp>
#include "../include/IPlayer.hpp"

using namespace std::literals;

using player_ptr  = std::unique_ptr<IPlayer>;
using func_player = std::function<void(IPlayer &)>;

const std::unordered_map<std::string, func_player> NO_ARGS_FUNCTIONS = {
    {"SET_WIN", [](IPlayer &x) { x.set_win(); }},
    {"SET_DRAW", [](IPlayer &x) { x.set_draw(); }},
    {"SET_LOSE", [](IPlayer &x) { x.set_lose(); }},
    {"SET_PLAYER_SYMBOL", [](IPlayer &x) { x.set_player_symbol(); }},
    {"ASK_FOR_MOVE", [](IPlayer &x) { x.ask_for_move(); }},
    {"SWAP_TURN", [](IPlayer &x) { x.swap_turn(); }},
};

class Client {
public:
    Client();
    ~Client() = default;

    void client_loop();

private:
    const std::array<char, 9> _convert_to_array(std::string str1);
    bool                      _parse_and_exec(sf::Packet &packet);
    bool                      _is_sock_done();
    sf::Socket::Status        _send_on_sock(std::string str);
    sf::TcpSocket             _sock;
    bool                      _played{false};
    player_ptr                _player;
};

#endif
