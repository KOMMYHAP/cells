#include "graveyard_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_graveyard_tag.h"

GraveyardSystem::GraveyardSystem(EcsWorld& world, CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

void GraveyardSystem::DoSystemUpdate()
{
    const auto graveyardedCells = _world->view<GraveyardTag, const CellPosition>();
    graveyardedCells.each([this](const EcsEntity /*id*/, const CellPosition position) {
        _locator->Reset(position);
    });

    _world->destroy(graveyardedCells.begin(), graveyardedCells.end());
}