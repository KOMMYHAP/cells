#include "world.h"

static_assert(std::is_signed_v<decltype(CellPosition::x)> && std::is_signed_v<decltype(CellPosition::x)>, "For vector math operations signed type is required");

World::World(uint32_t width, uint32_t height)
    : idSystem(width * height)
    , brainSystem(idSystem.GetCellsCountLimit())
    , typeSystem(idSystem.GetCellsCountLimit())
    , positionSystem(width, height)
    , cellFactory(virtualMachine, brainSystem)
    , graveyardSystem(idSystem.GetCellsCountLimit(), idSystem, typeSystem, positionSystem)
    , healthSystem(idSystem.GetCellsCountLimit(), graveyardSystem)
    , virtualMachine(brainSystem, typeSystem, healthSystem)
{
    virtualMachine.CreateProcedures(*this);
}

void World::Tick()
{
    idSystem.Iterate([this](const CellId id) {
        virtualMachine.Run(id);
    });
    graveyardSystem.Cleanup();
}
