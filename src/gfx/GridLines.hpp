#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

class GridLines {
public:
    GridLines();
    void draw_on(sf::RenderWindow &window);

private:
    std::vector<std::array<sf::Vertex, 2>> _lines;
};
