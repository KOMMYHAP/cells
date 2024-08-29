// #pragma once
//
// #include "simulation/simulation_ecs_system.h"
//
// #include "components/cell_type.h"
// #include "components/spawn_data.h"
// #include "simulation/simulation_virtual_machine.h"
//
// #include "random/random.h"
//
// class ICellFactory;
//
// class SpawnSystem final : public SimulationEcsSystem<SpawnSystem, const JustSpawned> {
// public:
//     SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, const SimulationVirtualMachine& vm);
//
//     void DoProcessComponents(CellId id, JustSpawned data);
//
// private:
//     void SpawnUnit(CellId id);
//     void SpawnFood(CellId id);
//
//     gsl::not_null<const SimulationVirtualMachine*> _simulationVm;
//     Random::Accessor _random;
// };