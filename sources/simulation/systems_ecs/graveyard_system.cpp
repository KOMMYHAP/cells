#include "graveyard_system.h"

#include "components/cell_position.h"
#include "components/graveyard_tag.h"

GraveyardSystem::GraveyardSystem(EcsWorld& world, CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

void GraveyardSystem::DoSystemUpdate()
{
    const auto graveyardedCells = _world->view<GraveyardTag, const CellPosition>();
    graveyardedCells.each([this](const CellId id, const CellPosition position) {
        _locator->Reset(position, id);
    });

    _world->destroy(graveyardedCells.begin(), graveyardedCells.end());
}