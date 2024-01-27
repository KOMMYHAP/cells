#pragma once

#include "systems/brain_system.h"
#include "systems/cell_factory.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/type_system.h"

class World {
public:
    World(uint32_t width, uint32_t height);

    void Tick();

public:
    IdSystem idSystem;
    BrainSystem brainSystem;
    TypeSystem typeSystem;
    SimulationVirtualMachine virtualMachine;
    PositionSystem positionSystem;
    CellFactory cellFactory;
    GraveyardSystem graveyardSystem;
    HealthSystem healthSystem;
};