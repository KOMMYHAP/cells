#include "world_render.h"
#include "render_profile_category.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/type_system.h"

WorldRender::WorldRender(Config&& config)
    : _config(std::move(config))
    , _vertexBuffer(sf::PrimitiveType::TrianglesStrip, sf::VertexBuffer::Static)
{
    const uint16_t cellsWidth = _config.positionSystem.GetWidth();
    const uint16_t cellsHeight = _config.positionSystem.GetHeight();

    const bool textureCreated = _texture.create(cellsWidth, cellsHeight);
    ASSERT(textureCreated);

    // We need shader only for custom texture now
    _config.fragmentShader->setUniform("texture", _texture);

    _textureData.resize(cellsWidth * cellsHeight);

    ASSERT(_vertexBuffer.isAvailable());
    std::array<sf::Vertex, 4> vertices {
        sf::Vertex { { _config.renderTargetSize.x - 1.0f, 0.0f }, { 1.0f, 0.0f } },
        sf::Vertex { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        sf::Vertex { { _config.renderTargetSize.x - 1.0f, _config.renderTargetSize.y - 1.0f }, { 1.0f, 1.0f } },
        sf::Vertex { { 0.0f, _config.renderTargetSize.y - 1.0f }, { 0.0f, 1.0f } },

    };
    const bool vertexBufferCreated = _vertexBuffer.create(vertices.size());
    ASSERT(vertexBufferCreated);

    const bool vertexBufferUpdated = _vertexBuffer.update(vertices.data());
    ASSERT(vertexBufferUpdated);
}

void WorldRender::Render(sf::RenderTarget& target, sf::RenderStates states)
{
    common::ProfileScope renderScope("Render", RenderProfileCategory);

    const uint32_t clearColor = GetColor(CellType::Dummy).toInteger();
    std::fill(_textureData.begin(), _textureData.end(), clearColor);

    _config.idSystem.Iterate([this](const CellId id) {
        ProcessCell(id);
    });
    _texture.update(reinterpret_cast<const uint8_t*>(_textureData.data()));

    states.shader = _config.fragmentShader.get();
    target.draw(_vertexBuffer, states);
}

sf::Color WorldRender::GetColor(CellType type) const
{
    switch (type) {
    case CellType::Unit:
        return _config.colors.at(0);
    case CellType::Food:
        return _config.colors.at(1);
    case CellType::Wall:
        return _config.colors.at(2);
    case CellType::Dummy:
        return _config.colors.at(3);
    default:
        UNREACHABLE("Unknown cell type!");
    }
}

void WorldRender::ProcessCell(CellId id)
{
    const CellPosition position = _config.positionSystem.Get(id);
    const uint16_t width = _config.positionSystem.GetWidth();
    const auto pixelIndex = position.y * width + position.x;
    ASSERT(pixelIndex < _textureData.size());

    const CellType cellType = _config.typeSystem.Get(id);

    uint32_t& pixel = _textureData[position.y * width + position.x];
    pixel = GetColor(cellType).toInteger();
}
