#pragma once
#include "components/cell_type.h"
#include "simulation_ecs_system.h"

#include "components/spawn_place.h"
#include "random/random.h"
#include "systems_ecs/simulation_virtual_machine.h"

class ICellFactory;

class SpawnSystem final : public SimulationEcsSystem<SpawnSystem, const SpawnPlace, CellType> {
public:
    SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, const SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, CellType type);

private:
    void SpawnUnit(CellId id);
    void SpawnFood(CellId id);
    
    gsl::not_null<const SimulationVirtualMachine*> _simulationVm;
    Random::Accessor _random;
};