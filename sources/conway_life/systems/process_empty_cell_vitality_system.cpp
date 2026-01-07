#include "components/generated/auto_neighbours_count.h"
#include "components/generated/auto_will_born_tag.h"
#include "generated/auto_process_empty_cell_vitality_system.h"

void ProcessEmptyCellVitalitySystem::DoProcessComponents(EcsEntity id, const NeighboursCount& neighboursCount)
{
    if (neighboursCount.count != 3) {
        return;
    }

    _ecsWorld->emplace_or_replace<WillBornTag>(id);
}