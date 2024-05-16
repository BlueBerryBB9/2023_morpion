#pragma once

#include <SFML/Network.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include "./IPlayer.hpp"

class StandaloneNetPlayer : public IPlayer {
public:
    StandaloneNetPlayer(char sym);
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
    std::string _receive_on_sock();
    void        _send_on_sock(std::string str);

    std::optional<unsigned int> _move_made{std::nullopt};
    bool                        _is_waiting_move{false};
    bool                        _is_its_turn{false};
    char                        _sym;
    sf::TcpSocket               _sock;
    sf::SocketSelector          _sect;
};
