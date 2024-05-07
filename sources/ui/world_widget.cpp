#include "world_widget.h"

#include "world.h"
#include "world_render.h"

WorldWidget::WorldWidget(sf::RenderTarget& renderTarget, World& world)
    : _renderTarget(renderTarget)
    , _worldRender(world.ModifySystems().Modify<WorldRender>())
{
}

WorldWidget::~WorldWidget() = default;

void WorldWidget::Draw()
{
    _worldRender.Render(_renderTarget, _rootStates);
}
