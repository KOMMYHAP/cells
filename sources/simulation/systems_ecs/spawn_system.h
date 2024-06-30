#pragma once
#include "simulation_ecs_system.h"

#include "components/spawn_place.h"
#include "random/random.h"
#include "systems_ecs/simulation_virtual_machine.h"

class ICellFactory;

class SpawnSystem final : public SimulationEcsSystem<SpawnSystem, const SpawnPlace> {
public:
    SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, const SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, SpawnPlace);
private:

    gsl::not_null<const SimulationVirtualMachine*> _simulationVm;
    Random::Accessor _random;
};