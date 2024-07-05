#pragma once

#include "cell_locator.h"
#include "simulation_ecs_system.h"
#include "simulation_virtual_machine.h"

#include "components/procedure_result.h"
#include "components/reproduction_direction.h"
#include "random/random.h"

class ReproductionSystem final : public SimulationEcsSystem<ReproductionSystem, const CellPosition, const ReproductionDirection, ProcedureResult> {
public:
    ReproductionSystem(EcsWorld& ecsWorld, const SimulationVirtualMachine& vm, const CellLocator& locator, Random::Accessor random);

    void DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, ProcedureResult& result);

private:
    gsl::not_null<const CellLocator*> _locator;
    gsl::not_null<const SimulationVirtualMachine*> _vm;
    Random::Accessor _random;
};
