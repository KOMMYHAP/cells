#pragma once

#include "procedures/procedure.h"
#include "systems_ecs/simulation_ecs_system.h"

class CellLocator;
class SimulationVirtualMachine;

class MoveProcedure final : public ProcedureBase {
public:
    MoveProcedure(EcsWorld& world);

    void Execute(ProcedureContext& context) override;

private:
    gsl::not_null<EcsWorld*> _world;
};