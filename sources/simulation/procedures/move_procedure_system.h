#pragma once

#include "simulation/cell_locator.h"
#include "systems_ecs/simulation_ecs_procedure.h"

class MoveProcedureSystem final : public EcsProcedureProxy<MoveProcedureSystem, CellPosition> {
public:
    using EcsProcedureProxy::ExecutionStatus;

    MoveProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, CellLocator& locator);
    ExecutionStatus ExecuteProcedure(CellId id, ProcedureContext& context, CellBrain& brain, CellPosition& position);

    EcsWorld& AccessEcsWorld() { return *_world; }
    SimulationVirtualMachine& AccessVirtualMachine() { return *_vm; }

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationVirtualMachine*> _vm;
    gsl::not_null<CellLocator*> _locator;
};