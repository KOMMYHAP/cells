#include "world_render.h"
#include "brain/brain_packed_data.h"
#include "field/field_iterator.h"
#include "render_profile_category.h"

WorldRender::WorldRender(Field& field, Config&& config)
    : _field(field)
    , _config(std::move(config))
    , _vertexBuffer(sf::PrimitiveType::TrianglesStrip, sf::VertexBuffer::Static)
{
    const uint16_t cellsWidth = _field.GetColumnsCount();
    const uint16_t cellsHeight = _field.GetRowsCount();

    const auto pixelsWidth = static_cast<float>(cellsWidth * _config.cellSize);
    const auto pixelsHeight = static_cast<float>(cellsHeight * _config.cellSize);

    if (!_texture.create(cellsWidth, cellsHeight)) {
        assert(false);
    }

    if (!_config.fragmentShader->isAvailable()) {
        assert(false);
    }

    _config.fragmentShader->setUniform("texture", _texture);
    _config.fragmentShader->setUniform("fieldResolution", sf::Vector2f { pixelsWidth, pixelsHeight });
    _config.fragmentShader->setUniform("cellsResolution", sf::Vector2f { (float)cellsWidth, (float)cellsHeight });
    _config.fragmentShader->setUniform("cellSize", static_cast<int>(_config.cellSize));

    std::cout << std::format("pixels ({}, {})", pixelsWidth, pixelsHeight) << std::endl;
    std::cout << std::format("cells ({}, {})", cellsWidth, cellsHeight) << std::endl;
    std::cout << std::format("pixel uv step ({}, {})", 1.0f / pixelsWidth, 1.0f / pixelsHeight) << std::endl;
    std::cout << std::format("cell uv step ({}, {})", 1.0f / cellsWidth, 1.0f / cellsHeight) << std::endl;
    std::cout << std::endl;

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

    _field.IterateByData([this](const CellId /*id*/, const Cell& cell) {
        ProcessCellByData(cell);
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

void WorldRender::ProcessCellByData(const Cell& cell)
{
    ConstBrain brain { cell };
    const CellInfo& info = brain.GetInfo();

    const uint16_t width = _field.GetColumnsCount();
    uint32_t& pixel = _textureData[info.position.y * width + info.position.x];

    const CellType cellType = brain.GetInfo().type;
    pixel = GetColor(cellType).toInteger();
}
