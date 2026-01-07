#include "generated/auto_collect_neighbours_count_system.h"

#include "components/generated/auto_has_life_tag.h"
#include "components/generated/auto_neighbours.h"
#include "components/generated/auto_neighbours_count.h"
#include "components/generated/auto_should_collect_neighbour.h"

void CollectNeighboursCountSystem::DoProcessComponents(EcsEntity id, const Neighbours& neighbours, NeighboursCount& neighboursCount)
{
    neighboursCount.count = 0;
    for (EcsEntity neighbour : neighbours.entities) {
        if (neighbour == InvalidEcsEntity) {
            continue;
        }

        neighboursCount.count += _ecsWorld->all_of<HasLifeTag>(neighbour) ? 1 : 0;
    }
    _ecsWorld->remove<ShouldCollectNeighbour>(id);
}