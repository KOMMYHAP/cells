#include "simulation_statistics_provider.h"

#include "cell_locator.h"
#include "systems_ecs/alive_cells_statistics_system.h"
#include "systems_ecs/spawn_places_statistics_system.h"

SimulationStatisticsProvider::SimulationStatisticsProvider(const Config& config)
    : _config(config)
{
}
size_t SimulationStatisticsProvider::GetCellsCount() const
{
    return _config.aliveCellsStats->GetAliveCellsCount();
}

size_t SimulationStatisticsProvider::GetSpawnPlacesCount() const
{
    return _config.spawnPlacesStats->GetSpawnPlacesCount();
}

size_t SimulationStatisticsProvider::GetCellsCapacity() const
{
    return _config.locator->GetHeight() * _config.locator->GetWidth();
}