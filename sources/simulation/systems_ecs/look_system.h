#pragma once

#include "simulation_ecs_system.h"

#include "simulation/cell_locator.h"
#include "simulation/simulation_virtual_machine.h"

#include "components/cell_brain.h"
#include "components/deferred_procedure_execution.h"
#include "components/look_direction.h"

class LookSystem final : public SimulationEcsSystem<LookSystem, const CellPosition, const LookDirection, CellBrain, DeferredProcedureExecution> {
public:
    LookSystem(EcsWorld& ecsWorld, CellLocator& locator, SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, CellPosition position, LookDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);

private:
    void Look(CellPosition position, LookDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure);
    
    gsl::not_null<CellLocator*> _locator;
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
