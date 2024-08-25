#pragma once

template <CellFactory Factory>
bool Spawner::TrySpawn(CellPosition targetPosition, Direction direction, Factory factory)
{
    const CellPosition childPosition = _locator->TryApplyDirection(targetPosition, direction);
    if (childPosition == InvalidCellPosition) {
        return false;
    }
    return TrySpawn(childPosition, factory);
}

template <CellFactory Factory>
bool Spawner::TrySpawn(CellPosition targetPosition, Factory factory)
{
    if (!CanSpawnAtPosition(targetPosition)) {
        return false;
    }
    CellBrain brain;
    if (const bool created = factory(brain); !created) {
        return false;
    }
    Spawn(brain, targetPosition);
    return true;
}