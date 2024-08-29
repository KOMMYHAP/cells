#pragma once

#include "simulation/cell_locator.h"
#include "systems_ecs/simulation_ecs_procedure.h"

class LookProcedureSystem final : public EcsProcedureProxy<LookProcedureSystem, CellPosition> {
public:
    LookProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, const CellLocator& locator);
    ExecutionStatus ExecuteProcedure(CellId id, ProcedureContext& context, CellBrain& brain, CellPosition position);
    
    EcsWorld& AccessEcsWorld() { return *_world; }
    SimulationVirtualMachine& AccessVirtualMachine() { return *_vm; }

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationVirtualMachine*> _vm;
    gsl::not_null<const CellLocator*> _locator;
};