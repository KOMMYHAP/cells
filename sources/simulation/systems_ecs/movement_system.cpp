#include "movement_system.h"

MovementSystem::MovementSystem(EcsWorld& ecsWorld, PositionSystem& currentPositions, PositionSystem& nextPositions)
    : SimulationEcsSystem(ecsWorld)
    , _currentPositionManager(&currentPositions)
    , _nextPositionManager(&nextPositions)
{
}

void MovementSystem::DoProcessComponents(const CellId id, CellPosition& position, const MoveDirection direction)
{
    const CellPosition nextPosition = _currentPositionManager->TryApplyDirection(position, direction);
    if (nextPosition == InvalidCellPosition) {
        return;
    }

    if (const CellId expectedTargetCell = _currentPositionManager->Find(nextPosition); expectedTargetCell != CellId::Invalid) {
        return;
    }

    EcsWorld& ecsWorld = AccessEcsWorld();
    ecsWorld.remove<MoveDirection>(id);

    {
        std::shared_lock _ { _positionMutex };
        if (const CellId actualTargetCell = _nextPositionManager->Find(nextPosition); actualTargetCell != CellId::Invalid) {
            return;
        }
    }

    {
        std::unique_lock _ { _positionMutex };
        _currentPositionManager->Set(id, position);
    }

    position = nextPosition;
}