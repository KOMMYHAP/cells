#include "world_render.h"
#include "render_profile_category.h"

WorldRender::WorldRender(World& world, Config&& config)
    : _world(world)
    , _config(std::move(config))
    , _vertexBuffer(sf::PrimitiveType::TrianglesStrip, sf::VertexBuffer::Static)
{
    const uint16_t cellsWidth = _world.GetWidth();
    const uint16_t cellsHeight = _world.GetHeight();

    const auto pixelsWidth = static_cast<float>(cellsWidth * _config.cellSize);
    const auto pixelsHeight = static_cast<float>(cellsHeight * _config.cellSize);

    if (!_texture.create(cellsWidth, cellsHeight)) {
        assert(false);
    }

    if (!_config.fragmentShader->isAvailable()) {
        assert(false);
    }

    _config.fragmentShader->setUniform("texture", _texture);

    _textureData.resize(cellsWidth * cellsHeight);

    if (!_vertexBuffer.isAvailable()) {
        assert(false);
    }
    std::array<sf::Vertex, 4> vertices {
        sf::Vertex { { pixelsWidth - 1, 0.0f }, { 1.0f, 0.0f } },
        sf::Vertex { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        sf::Vertex { { pixelsWidth - 1, pixelsHeight - 1 }, { 1.0f, 1.0f } },
        sf::Vertex { { 0.0f, pixelsHeight - 1 }, { 0.0f, 1.0f } },

    };
    if (!_vertexBuffer.create(vertices.size())) {
        assert(false);
    }
    if (!_vertexBuffer.update(vertices.data())) {
        assert(false);
    }
}

void WorldRender::Render(sf::RenderTarget& target, sf::RenderStates states)
{
    common::ProfileScope renderScope("Render", RenderProfileCategory);

    const uint32_t clearColor = GetColor(CellType::Dummy).toInteger();
    std::fill(_textureData.begin(), _textureData.end(), clearColor);

    _world.idSystem.Iterate([this](const CellId id) {
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
        assert(false);
        return GetColor(CellType::Dummy);
    }
}

void WorldRender::ProcessCell(CellId id)
{
    const CellPosition position = _world.positionSystem.Get(id);
    if (position == InvalidCellPosition) {
        return;
    }

    const uint16_t width = _world.GetWidth();
    uint32_t& pixel = _textureData[position.y * width + position.x];

    const CellType cellType = _world.typeSystem.Get(id);
    pixel = GetColor(cellType).toInteger();
}
