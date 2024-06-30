#include "movement_system.h"

MovementSystem::MovementSystem(EcsWorld& ecsWorld, CellLocator& currentPositions)
    : SimulationEcsSystem(ecsWorld)
    , _currentPositionManager(&currentPositions)
{
}

void MovementSystem::DoProcessComponents(const CellId id, CellPosition& position, const MoveDirection direction)
{
    const CellPosition nextPosition = _currentPositionManager->TryApplyDirection(position, direction);
    if (nextPosition == InvalidCellPosition) {
        return;
    }
    if (const CellId targetCell = _currentPositionManager->Find(nextPosition); targetCell != CellId::Invalid) {
        return;
    }

    EcsWorld& ecsWorld = AccessEcsWorld();
    ecsWorld.remove<MoveDirection>(id);

    _currentPositionManager->Move(position, nextPosition);
    position = nextPosition;
}