#include "simulation_statistics_provider.h"

#include "cell_locator.h"

SimulationStatisticsProvider::SimulationStatisticsProvider(const CellLocator& locator)
    : _cellsCapacity(locator.GetHeight() * locator.GetWidth())
{
}