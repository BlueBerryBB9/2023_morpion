#pragma once
#include <future>
#include <optional>
#include "IPlayer.hpp"

class TermPlayer : public IPlayer {
public:
    TermPlayer(char sym) : _sym{sym} {};
    ~TermPlayer();
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
    void set_sym(char sym) override;
    void wait() override;
    void play_again() override;
    void set_phase(PLAYER_PHASE phase) override;
    std::optional<unsigned int> get_move() override;

private:
    std::optional<unsigned int> _move_made{std::nullopt};
    bool                        _is_its_turn{false};
    std::future<int>            _future;
    std::future<char>           _future2;
    bool                        _can_ask_again{true};
    char                        _sym;
    bool                        _replay_mode{false};
    bool                        _is_replaying{false};
    PLAYER_PHASE                _phase{PLAYER_PHASE::waiting_opponent};
};
