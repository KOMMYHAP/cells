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
        uint8_t cellSize;
    };

    WorldRender(Config&& config, PositionSystem& positionSystem, IdSystem& idSystem, TypeSystem& typeSystem);

    void Render(sf::RenderTarget& target, sf::RenderStates states);

private:
    void ProcessCell(CellId id);
    sf::Color GetColor(CellType type) const;

    Config _config;
    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;

    PositionSystem& _positionSystem;
    IdSystem& _idSystem;
    TypeSystem& _typeSystem;
};