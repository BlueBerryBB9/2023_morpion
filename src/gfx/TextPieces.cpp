#include "TextPieces.hpp"

TextPieces::TextPieces() : _font{}, _pieces{}
{
    if (!_font.loadFromFile("assets/comics_sans.ttf")) {
        std::cerr << "failed to load font" << std::endl;
    }
    int x{39};
    int y{39};
    for (auto &text : _pieces) {
        text.setFont(_font);
        text.setString("x");
        text.setCharacterSize(24);
        text.setPosition(sf::Vector2f(x, y));
        x += 100;
        if (x > 239) {
            x = 39;
            y += 100;
        }
    }
}

void TextPieces::set_from(const std::array<char, 9> &game_array)
{
    auto text_tile = _pieces.begin();
    auto game_tile = game_array.begin();
    while (game_tile != game_array.end() && text_tile != _pieces.end()) {
        if (*game_tile == '.')
            text_tile->setString(" ");
        else
            text_tile->setString(*game_tile);
        std::advance(text_tile, 1);
        std::advance(game_tile, 1);
    }
}

void TextPieces::draw_on(sf::RenderWindow &window)
{
    for (auto &text : _pieces) {
        window.draw(text);
    }
}
