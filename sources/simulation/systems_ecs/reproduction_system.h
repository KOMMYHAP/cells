#pragma once

#include "simulation/cell_locator.h"

#include "cell_factories/cell_factory_interface.h"
#include "simulation/simulation_virtual_machine.h"
#include "simulation_ecs_system.h"

#include "components/deferred_procedure_execution.h"
#include "components/reproduction_direction.h"

class ReproductionSystem final : public SimulationEcsSystem<ReproductionSystem, const CellPosition, const ReproductionDirection, CellBrain, DeferredProcedureExecution> {
public:
    ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator, ICellFactory& factory);

    void DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);

private:
    void MakeClone(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);
    
    gsl::not_null<ICellFactory*> _factory;
    gsl::not_null<const CellLocator*> _locator;
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
