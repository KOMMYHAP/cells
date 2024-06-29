#pragma once
#include "simulation_ecs_system.h"

#include "components/cell_position.h"
#include "components/spawn_place.h"
#include "random/random.h"
#include "systems_ecs/simulation_virtual_machine.h"

class ICellFactory;

class SpawnSystem final : public SimulationEcsSystem<SpawnSystem, CellPosition, SpawnPlace> {
public:
    SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, CellPosition position, SpawnPlace);
private:

    gsl::not_null<SimulationVirtualMachine*> _simulationVm;
    Random::Accessor _random;
};