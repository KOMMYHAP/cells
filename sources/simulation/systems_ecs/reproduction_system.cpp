#include "reproduction_system.h"

#include "cell_factories/patrol_cell.h"
#include "components/procedure_result.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, const SimulationVirtualMachine& vm, const CellLocator& locator, Random::Accessor random)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
    , _random(random)
{
}

void ReproductionSystem::DoProcessComponents(CellId id, const CellPosition position, const ReproductionDirection direction, ProcedureResult& result)
{
    const CellPosition childPosition = _locator->TryApplyDirection(position, direction.value);
    if (childPosition == InvalidCellPosition) {
        result.context.MarkProcedureAsInvalid();
        return;
    }

    if (const CellId targetId = _locator->Find(childPosition); targetId != CellId::Invalid) {
        result.context.MarkProcedureAsInvalid();
        return;
    }

    EcsWorld& world = AccessEcsWorld();
    const CellId child = world.create();
    world.emplace<CellBrain>(child, MakePatrolCell(*_vm));
    world.emplace<CellPosition>(child, childPosition);
}