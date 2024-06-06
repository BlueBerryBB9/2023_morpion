#pragma once

#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <chrono>
#include <memory>
#include "./IPlayer.hpp"

class StandaloneNetPlayer : public IPlayer {
public:
    StandaloneNetPlayer(char sym);
    StandaloneNetPlayer(char sym, std::unique_ptr<sf::TcpSocket> &sock);
    ~StandaloneNetPlayer();

    void set_win() override;
    void set_draw() override;
    void set_lose() override;
    void set_board_state(const std::array<char, 9> &board) override;
    bool is_done() override;
    void ask_for_move() override;
    void set_turn(bool your_turn) override;
    void swap_turn() override;
    void process_events() override;
    void set_player_symbol() override;
    char get_sym() override;
    std::optional<unsigned int> get_move() override;

private:
    std::optional<int> _receive_on_sock();
    sf::Socket::Status _send_on_sock(std::string str);
    sf::Socket::Status _send_on_sock(sf::Packet &packet);
    sf::Socket::Status _send_on_sock();
    bool               _connection_closed();

    std::optional<unsigned int>    _move_made{std::nullopt};
    bool                           _is_its_turn{false};
    bool                           _can_ask_again{true};
    bool                           _done{false};
    char                           _sym;
    std::unique_ptr<sf::TcpSocket> _sock;
    sf::SocketSelector             _sect;

    std::chrono::time_point<std::chrono::steady_clock> _last_clock;
};
