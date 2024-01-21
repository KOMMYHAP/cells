#pragma once
#include "world.h"

class WorldRender {
public:
    struct Config {
        std::unique_ptr<sf::Shader> fragmentShader;
        std::vector<sf::Color> colors;
        uint8_t cellSize;
    };

    WorldRender(World& world, Config&& config);

    void Render(sf::RenderTarget& target, sf::RenderStates states);

private:
    void ProcessCell(CellId id);
    sf::Color GetColor(CellType type) const;

    World& _world;
    Config _config;
    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;
};