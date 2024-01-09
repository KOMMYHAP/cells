#include "world_render.h"
#include "field/field_iterator.h"

WorldRender::WorldRender(Field& field, CellRender::Config cellRenderConfig)
    : _field(field)
    , _cellRender(std::move(cellRenderConfig))
{
}

void WorldRender::Render(sf::RenderTarget& target)
{
    for (const Cell& cell : _field) {
        _cellRender.Render(target, cell);
    }
}
