#include "alive_cells_statistics_system.h"

#include "components/generated/cell_unit_tag.h"

AliveCellsStatisticsSystem::AliveCellsStatisticsSystem(EcsWorld& ecsWorld, SimulationStatisticsProvider& stats)
    : _world(&ecsWorld)
    , _stats(&stats)
{
}

void AliveCellsStatisticsSystem::DoSystemUpdate()
{
    const size_t aliveCells = _world->view<const CellUnitTag>(ExcludeGraveyardedCells).size_hint();
    _stats->SetCellsCount(aliveCells);
}