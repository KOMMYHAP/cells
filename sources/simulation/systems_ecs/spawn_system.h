﻿#pragma once
#include "ecs_simulation_system.h"

#include "components/cell_position.h"
#include "systems/simulation_virtual_machine.h"

class ICellFactory;

class SpawnSystem final : public EcsSimulationSystem<SpawnSystem, const CellPosition> {
public:
    SpawnSystem(EcsWorld& ecsWorld, ICellFactory& cellFactory);

private:
    void DoProcessComponents(CellId id, CellPosition position);

    gsl::not_null<SimulationVirtualMachine*> _simulationVm;
};