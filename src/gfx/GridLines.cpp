#include "GridLines.hpp"

GridLines::GridLines() : _lines{}
{
    _lines.push_back({sf::Vertex(sf::Vector2f(99.f, 0.f)),
                      sf::Vertex(sf::Vector2f(99.f, 299.f))});
    _lines.push_back({sf::Vertex(sf::Vector2f(199.f, 0.f)),
                      sf::Vertex(sf::Vector2f(199.f, 299.f))});
    _lines.push_back({sf::Vertex(sf::Vector2f(0.f, 99.f)),
                      sf::Vertex(sf::Vector2f(299.f, 99.f))});
    _lines.push_back({sf::Vertex(sf::Vector2f(0.f, 199.f)),
                      sf::Vertex(sf::Vector2f(299.f, 199.f))});
}

void GridLines::draw_on(sf::RenderWindow &window)
{
    for (const auto &vectrex : _lines) {
        window.draw(vectrex.data(), 2, sf::Lines);
    }
}
