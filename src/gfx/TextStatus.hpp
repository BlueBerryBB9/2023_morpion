#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class TextStatus {
public:
    TextStatus();
    void set_text(std::string text);
    void draw_on(sf::RenderWindow &window);

private:
    sf::Font           _font;
    sf::Text           _text;
    sf::RectangleShape _text_clear_box;
};
