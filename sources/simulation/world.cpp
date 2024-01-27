#include "world.h"

World::World(uint32_t width, uint32_t height)
    : idSystem(width * height)
    , brainSystem(idSystem.GetCellsCountLimit())
    , typeSystem(idSystem.GetCellsCountLimit())
    , positionSystem(width, height)
    , cellFactory(simulationVm, brainSystem)
    , graveyardSystem(idSystem.GetCellsCountLimit(), idSystem, typeSystem, positionSystem)
    , healthSystem(idSystem.GetCellsCountLimit(), graveyardSystem)
    , simulationVm(brainSystem, typeSystem, healthSystem)
{
    simulationVm.CreateProcedures(*this);
}

void World::Tick()
{
    // Process brain of each cell.
    idSystem.Iterate([this](const CellId id) {
        simulationVm.Run(id);
    });

    // Cleanup world systems from dead cells.
    graveyardSystem.Cleanup();
}
