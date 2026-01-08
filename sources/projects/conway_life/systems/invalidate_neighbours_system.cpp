#include "generated/auto_invalidate_neighbours_system.h"

#include "components/generated/auto_neighbours.h"
#include "components/generated/auto_neighbours_count.h"
#include "components/generated/auto_should_collect_neighbour.h"

void InvalidateNeighboursSystem::DoProcessComponents(EcsEntity id, const Neighbours& neighbours)
{
    _ecsWorld->emplace_or_replace<ShouldCollectNeighbour>(id);

    for (const EcsEntity neighbour : neighbours.entities) {
        if (neighbour == InvalidEcsEntity) {
            continue;
        }
        _ecsWorld->emplace_or_replace<ShouldCollectNeighbour>(neighbour);
    }
}