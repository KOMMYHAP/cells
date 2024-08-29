#pragma once

#include "cell_factories/random_cell_factory.h"
#include "simulation/cell_locator.h"
#include "simulation/spawner.h"
#include "systems_ecs/simulation_ecs_procedure.h"

class RandomCellSpawnProcedureSystem final : public EcsProcedureProxy<RandomCellSpawnProcedureSystem, const CellPosition> {
public:
    using EcsProcedureProxy::ExecutionStatus;

    RandomCellSpawnProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, const CellLocator& locator, Spawner& spawner, RandomCellFactory& factory);
    ExecutionStatus ExecuteProcedure(CellId id, ProcedureContext& context, CellBrain& brain, CellPosition position);

    EcsWorld& AccessEcsWorld() { return *_world; }
    SimulationVirtualMachine& AccessVirtualMachine() { return *_vm; }

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationVirtualMachine*> _vm;
    gsl::not_null<const CellLocator*> _locator;
    gsl::not_null<Spawner*> _spawner;
    gsl::not_null<RandomCellFactory*> _factory;
};