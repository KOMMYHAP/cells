#pragma once

#include "cell_render.h"
#include "field/field.h"

enum class CellType : uint8_t;

class WorldRender {
public:
    WorldRender(Field& field, CellRender::Config cellRenderConfig);

    void Render(sf::RenderTarget& target, sf::RenderStates states);

private:
    void ProcessCellByPosition(const CellId id, const CellPosition& position);
    void ProcessCellByData(const Cell& cell);
    sf::Color GetColor(CellType type) const;

    const Field& _field;
    CellRender _cellRender;
    CellRender::Config _config;
    sf::Texture _texture;
    sf::RectangleShape _shape;
    sf::Shader _shader;
    std::vector<uint32_t> _textureData;
};