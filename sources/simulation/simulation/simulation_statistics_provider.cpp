#include "simulation_statistics_provider.h"

SimulationStatisticsProvider::SimulationStatisticsProvider(CellLocator& locator, AliveCellsStatisticsSystem& aliveCellsSystem)
    : _aliveCellsSystem(&aliveCellsSystem)
    , _cellLocator(&locator)
{
}