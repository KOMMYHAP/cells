#include "look_system.h"

#include "components/cell_type.h"

LookSystem::LookSystem(EcsWorld& ecsWorld, CellLocator& locator)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
{
}

void LookSystem::DoProcessComponents(const CellPosition position, const LookDirection direction, ProcedureResult& result)
{
    const CellPosition lookPosition = _locator->TryApplyDirection(position, direction.value);
    if (lookPosition == InvalidCellPosition) {
        result.context.TryPushResult(CellType::Wall);
        return;
    }

    if (const CellId anotherCell = _locator->Find(lookPosition); anotherCell == CellId::Invalid) {
        result.context.TryPushResult(CellType::Dummy);
        return;
    }

    result.context.TryPushResult(CellType::Unit);
}