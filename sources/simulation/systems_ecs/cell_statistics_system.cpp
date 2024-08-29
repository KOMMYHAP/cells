#include "cell_statistics_system.h"

#include "components/cell_unit_tag.h"

AliveCellsStatisticsSystem::AliveCellsStatisticsSystem(EcsWorld& ecsWorld)
    : _world(&ecsWorld)
{
}
void AliveCellsStatisticsSystem::DoSystemUpdate()
{
    _aliveCells = _world->view<CellUnitTag>(ExcludeGraveyardedCells).size_hint();
}