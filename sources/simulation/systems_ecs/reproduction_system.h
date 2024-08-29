#pragma once
#include "simulation_ecs_system.h"

#include "cell_factories/random_cell_factory.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_virtual_machine.h"

#include "components/deferred_procedure_execution.h"
#include "components/random_cell_spawn_tag.h"
#include "simulation/spawner.h"

class ReproductionSystem final : public SimulationEcsSystem<ReproductionSystem, const CellPosition, const ReproductionDirection, CellBrain, DeferredProcedureExecution> {
public:
    ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator, RandomCellFactory& factory, Spawner& spawner);

    void DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);

private:
    void MakeClone(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);

    gsl::not_null<Spawner*> _spawner;
    gsl::not_null<RandomCellFactory*> _factory;
    gsl::not_null<const CellLocator*> _locator;
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
