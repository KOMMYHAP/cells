#include "world_render.h"
#include "brain/brain_packed_data.h"
#include "field/field_iterator.h"
#include "render_profile_category.h"

WorldRender::WorldRender(Field& field, CellRender::Config cellRenderConfig)
    : _field(field)
    , _cellRender(cellRenderConfig)
    , _config(cellRenderConfig)
{
    const bool textureCreated = _texture.create(_field.GetColumnsCount(), _field.GetRowsCount());
    assert(textureCreated);

    _shape.setSize({ static_cast<float>(_field.GetColumnsCount()), static_cast<float>(_field.GetRowsCount()) });
    _shape.setTexture(&_texture, true);

    _textureData.resize(_field.GetColumnsCount() * _field.GetRowsCount(), sf::Color::Transparent.toInteger());
}

void WorldRender::Render(sf::RenderTarget& target, sf::RenderStates states)
{
    common::ProfileScope renderScope("Render", RenderProfileCategory);

    //    _field.IterateByPositions([this](const CellId id, const CellPosition& position) {
    //        ProcessCellByPosition(id, position);
    //    });

    _field.IterateByData([this](const CellId /*id*/, const Cell& cell) {
        ProcessCellByData(cell);
    });

    //    sf::RenderStates states { &_shader };
    _texture.update(reinterpret_cast<const uint8_t*>(_textureData.data()));

    target.draw(_shape, states);
}

void WorldRender::ProcessCellByPosition(const CellId id, const CellPosition& position)
{
    const uint16_t width = _field.GetColumnsCount();
    uint32_t& pixel = _textureData[position.y * width + position.x];

    if (id == CellId::Invalid) {
        pixel = sf::Color::Transparent.toInteger();
        return;
    }

    ConstBrain brain { _field.Get(id) };
    CellType cellType = brain.GetInfo().type;
    pixel = GetColor(cellType).toInteger();
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
