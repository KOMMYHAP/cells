#pragma once

#include "components/cell_id.h"
#include "components/cell_type.h"

class PositionSystem;
class IdSystem;
class TypeSystem;

class WorldRender {
public:
    struct Config {
        std::unique_ptr<sf::Shader> fragmentShader;
        std::vector<sf::Color> colors;
        sf::Vector2u renderTargetSize;

        PositionSystem& positionSystem;
        IdSystem& idSystem;
        TypeSystem& typeSystem;
    };

    WorldRender(Config&& config);

    void Render(sf::RenderTarget& target, sf::RenderStates states);

private:
    void ProcessCell(CellId id);
    sf::Color GetColor(CellType type) const;

    Config _config;
    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;
};