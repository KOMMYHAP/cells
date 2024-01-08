#include "world_render.h"

WorldRender::WorldRender(Field& field)
    : _field(field)
{
}

void WorldRender::Render(sf::RenderTarget& target)
{
    for (const Cell& cell : _field) {
        _cellRender.Render(target, cell);
    }
}
