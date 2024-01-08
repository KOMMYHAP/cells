#pragma once

struct Cell;

class CellRender {
public:
    CellRender();

    void Render(sf::RenderTarget& target, const Cell& cell);

private:
    const float _paddingLeft;
    const float _paddingTop;
    const float _cellSize;
    const sf::Vector2f _fieldOffset;
    std::vector<sf::Color> _colors;
};