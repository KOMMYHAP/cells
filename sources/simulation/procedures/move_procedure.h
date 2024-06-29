#pragma once

#include "procedures/procedure.h"
#include "simulation_ecs.h"

class PositionSystem;
class SimulationVirtualMachine;

class MoveProcedure final : public ProcedureBase {
public:
    MoveProcedure(EcsWorld& world);

    void Execute(ProcedureContext& context) override;

private:
    gsl::not_null<EcsWorld*> _world;
};