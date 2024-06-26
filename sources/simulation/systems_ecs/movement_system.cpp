#include "movement_system.h"

MovementSystem::MovementSystem(EcsWorld& ecsWorld, PositionSystem& currentPositions, PositionSystem& nextPositions)
    : EcsSimulationSystem(ecsWorld)
    , _currentPositionManager(&currentPositions)
    , _nextPositionManager(&nextPositions)
{
}

void MovementSystem::DoProcessComponents(CellId id, CellPosition position, MoveDirection direction)
{
    const CellPosition nextPosition = _currentPositionManager->TryApplyDirection(position, direction);
    if (nextPosition == InvalidCellPosition) {
        return;
    }

    const CellId targetCell = _currentPositionManager->Find(nextPosition);
    if (targetCell != CellId::Invalid) {
        return;
    }

    EcsWorld& ecsWorld = AccessEcsWorld();
    ecsWorld.remove<MoveDirection>(id);
    ecsWorld.emplace_or_replace<CellPosition>(id, position);

    std::scoped_lock _ { _nextPositionMutex };
    _nextPositionManager->Set(id, nextPosition);
}