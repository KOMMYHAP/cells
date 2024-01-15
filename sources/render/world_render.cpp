#include "world_render.h"
#include "field/field_iterator.h"
#include "render_profile_category.h"

WorldRender::WorldRender(Field& field, CellRender::Config cellRenderConfig)
    : _field(field)
    , _cellRender(std::move(cellRenderConfig))
{
}

void WorldRender::Render(sf::RenderTarget& target)
{
    common::ProfileScope renderScope("Render", RenderProfileCategory);
    for (const Cell& cell : _field) {
        _cellRender.Render(target, cell);
    }
}
