#pragma once

#include "cell_factories/random_cell_factory.h"
#include "components/generated/auto_cell_energy_decrease.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_ecs_procedure.h"
#include "simulation/spawner.h"

class RandomCellSpawnProcedureSystem final : public EcsProcedureProxy<RandomCellSpawnProcedureSystem, const CellPosition, CellEnergyDecrease> {
public:
    using EcsProcedureProxy::ExecutionStatus;

    RandomCellSpawnProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, const CellLocator& locator, Spawner& spawner, RandomCellFactory& factory);
    ExecutionStatus ExecuteProcedure(EcsEntity id, ProcedureContext& context, CellBrain& brain, CellPosition position, CellEnergyDecrease & energyChange);

    EcsWorld& AccessEcsWorld() { return *_world; }
    SimulationVirtualMachine& AccessVirtualMachine() { return *_vm; }

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationVirtualMachine*> _vm;
    gsl::not_null<const CellLocator*> _locator;
    gsl::not_null<Spawner*> _spawner;
    gsl::not_null<RandomCellFactory*> _factory;
};