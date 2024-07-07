#pragma once

#include "components/cell_brain.h"
#include "simulation_ecs_system.h"

#include "simulation/cell_locator.h"

#include "components/deferred_procedure_execution.h"
#include "components/look_direction.h"
#include "simulation/simulation_virtual_machine.h"

class LookSystem final : public SimulationEcsSystem<LookSystem, const CellPosition, const LookDirection, CellBrain, DeferredProcedureExecution> {
public:
    explicit LookSystem(EcsWorld& ecsWorld, CellLocator& locator, SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, CellPosition position, LookDirection direction, CellBrain& brain, DeferredProcedureExecution);

private:
    gsl::not_null<CellLocator*> _locator;
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
