#pragma once

#include "systems/brain_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/type_system.h"

class World {
public:
    World(uint32_t width, uint32_t height);

    uint16_t GetWidth() const { return _width; }
    uint16_t GetHeight() const { return _height; }
    uint32_t GetCapacity() const { return GetWidth() * GetHeight(); }

public:
    SimulationVirtualMachine virtualMachine;
    BrainSystem brainSystem;
    PositionSystem positionSystem;
    IdSystem idSystem;
    TypeSystem typeSystem;

private:
    uint16_t _width { 0 };
    uint16_t _height { 0 };
};