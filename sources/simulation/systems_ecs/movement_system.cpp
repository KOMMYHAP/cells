#include "movement_system.h"

MovementSystem::MovementSystem(EcsWorld& ecsWorld, CellLocator& currentPositions)
    : SimulationEcsSystem(ecsWorld)
    , _cellsLocator(&currentPositions)
{
}

void MovementSystem::DoProcessComponents(const CellId id, CellPosition& position, const MoveDirection direction)
{
    EcsWorld& ecsWorld = AccessEcsWorld();
    ecsWorld.remove<MoveDirection>(id);
    
    const CellPosition nextPosition = _cellsLocator->TryApplyDirection(position, direction.value);
    if (nextPosition == InvalidCellPosition) {
        return;
    }
    if (const CellId targetCell = _cellsLocator->Find(nextPosition); targetCell != CellId::Invalid) {
        return;
    }

    _cellsLocator->Replace(position, nextPosition);
    position = nextPosition;
}