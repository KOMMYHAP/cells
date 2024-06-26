#pragma once
#include "ecs_simulation_system.h"

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "components/move_direction.h"
#include "systems/position_system.h"

class ICellFactory;

class MovementSystem final : public EcsSimulationSystem<MovementSystem, CellPosition, MoveDirection> {
public:
    MovementSystem(EcsWorld& ecsWorld, PositionSystem& currentPositions, PositionSystem& nextPositions);

private:
    void DoProcessComponents(CellId id, CellPosition position, MoveDirection direction);

    gsl::not_null<PositionSystem*> _currentPositionManager;
    gsl::not_null<PositionSystem*> _nextPositionManager;
    std::mutex _nextPositionMutex;
};