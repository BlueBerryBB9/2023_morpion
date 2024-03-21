#include "TextStatus.hpp"

TextStatus::TextStatus()
    : _font{},
      _text{"initialising", _font, 24},
      _text_clear_box(sf::Vector2f{0, 300.f})
{
    if (!_font.loadFromFile("assets/comics_sans.ttf")) {
        std::cerr << "failed to load font" << std::endl;
    }
    _text.setPosition(0, 300);
    _text_clear_box.setFillColor(sf::Color::Black);
    _text_clear_box.setPosition(sf::Vector2f(0, 300));
    _text_clear_box.setSize(sf::Vector2f{300.f, 40.f});
}

void TextStatus::set_text(std::string text)
{
    _text.setString(text);
}

void TextStatus::draw_on(sf::RenderWindow &window)
{
    window.draw(_text_clear_box);
    window.draw(_text);
}
