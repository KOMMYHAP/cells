#pragma once

#include "cell_locator.h"
#include "simulation_ecs_system.h"

#include "components/look_direction.h"
#include "components/procedure_result.h"

class LookSystem final : public SimulationEcsSystem<LookSystem, const CellPosition, const LookDirection, ProcedureResult> {
public:
    explicit LookSystem(EcsWorld& ecsWorld, CellLocator& locator);

    void DoProcessComponents(CellId id, CellPosition position, LookDirection direction, ProcedureResult& result);

private:
    gsl::not_null<CellLocator*> _locator;
};
