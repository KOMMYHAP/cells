#pragma once
#include "cell_locator.h"
#include "systems_ecs/alive_cells_statistics_system.h"

class SimulationStatisticsProvider {
public:
    SimulationStatisticsProvider(CellLocator& locator, AliveCellsStatisticsSystem& aliveCellsSystem);

    size_t GetCellsCount() const { return _aliveCellsSystem->GetAliveCellsCount(); }
    size_t GetCellsCapacity() const { return _cellLocator->GetHeight() * _cellLocator->GetWidth(); }

private:
    gsl::not_null<AliveCellsStatisticsSystem*> _aliveCellsSystem;
    gsl::not_null<CellLocator*> _cellLocator;
};