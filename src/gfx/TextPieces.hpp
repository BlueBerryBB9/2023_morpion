#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <iterator>

class TextPieces {
public:
    TextPieces();
    void set_from(const std::array<char, 9> &game_array);
    void draw_on(sf::RenderWindow &window);

private:
    sf::Font                _font;
    std::array<sf::Text, 9> _pieces;
};
