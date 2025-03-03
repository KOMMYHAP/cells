#include "generated/auto_alive_cells_statistics_system.h"

#include "simulation/simulation_statistics_provider.h"

#include "components/generated/auto_cell_unit_tag.h"
#include "components/generated/auto_graveyard_tag.h"

void AliveCellsStatisticsSystem::DoSystemUpdate()
{
    const size_t cellsCount = _ecsWorld->view<CellUnitTag>(entt::exclude_t<GraveyardTag> {}).size_hint();
    _simulationStatisticsProvider->SetCellsCount(static_cast<int64_t>(cellsCount));
}