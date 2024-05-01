#include "world_widget.h"

#include "world.h"
#include "world_render.h"

WorldWidget::WorldWidget(sf::RenderTarget& renderTarget, World& world)
    : _renderTarget(renderTarget)
{
    WorldRender::Config renderConfig {
        {},{},{},
        world.
    };
    _worldRender = std::make_unique<WorldRender>(std::move(renderConfig));
}

WorldWidget::~WorldWidget() = default;

void WorldWidget::Draw()
{
}
