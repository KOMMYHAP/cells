#pragma once

struct Cell;

class CellRender {
public:
    struct Config {
        float paddingLeft;
        float paddingTop;
        float cellSize;
        sf::Vector2f fieldOffset;
        std::vector<sf::Color> colors;
    };

    CellRender(Config config);

    void Render(sf::RenderTarget& target, const Cell& cell);

private:
    const Config _config;
};