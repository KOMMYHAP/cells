#pragma once
#include "cell_render.h"
#include "field/field.h"
#include <filesystem>

enum class CellType : uint8_t;

class WorldRender {
public:
    struct Config {
        std::unique_ptr<sf::Shader> fragmentShader;
        std::vector<sf::Color> colors;
        uint8_t cellSize;
    };

    WorldRender(Field& field, Config && config);

    void Render(sf::RenderTarget& target, sf::RenderStates states);

private:
    void ProcessCellByData(const Cell& cell);
    sf::Color GetColor(CellType type) const;

    const Field& _field;
    Config _config;
    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;
};