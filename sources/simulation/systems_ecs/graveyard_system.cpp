#include "systems_ecs/generated/auto_graveyard_system.h"

#include "simulation/cell_locator.h"

void GraveyardSystem::DoProcessComponents(EcsEntity id, const CellPosition& cellPosition)
{
    _cellLocator->Reset(cellPosition);

    // todo: we can destroy a batch of entities, do we need it for performance?
    _ecsWorld->destroy(id);
}