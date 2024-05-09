#include "world_widget.h"

#include "world.h"
#include "world_render.h"

WorldWidget::WorldWidget(sf::RenderTarget& renderTarget, WorldRender& worldRender, sf::Vector2f renderAreaOffset)
    : _renderTarget(renderTarget)
    , _worldRender(worldRender)
{
    _rootStates.transform.translate(renderAreaOffset);
}

WorldWidget::~WorldWidget() = default;

void WorldWidget::Draw()
{
    _worldRender.Render(_renderTarget, _rootStates);
}
