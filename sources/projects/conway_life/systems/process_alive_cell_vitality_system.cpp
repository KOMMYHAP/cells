#include "components/generated/auto_neighbours_count.h"
#include "components/generated/auto_will_die_tag.h"
#include "generated/auto_process_alive_cell_vitality_system.h"

void ProcessAliveCellVitalitySystem::DoProcessComponents(EcsEntity id, const NeighboursCount& neighboursCount)
{
    const int32_t count = neighboursCount.count;
    if (count == 2 || count == 3) {
        return;
    }

    _ecsWorld->emplace_or_replace<WillDieTag>(id);
}