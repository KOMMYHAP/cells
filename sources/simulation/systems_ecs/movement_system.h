﻿#pragma once
#include "simulation_ecs_system.h"

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "components/move_direction.h"
#include "systems_ecs/position_system.h"

class ICellFactory;

class MovementSystem final : public SimulationEcsSystem<MovementSystem, CellPosition, MoveDirection> {
public:
    MovementSystem(EcsWorld& ecsWorld, PositionSystem& currentPositions, PositionSystem& nextPositions);

    void DoProcessComponents(CellId id, CellPosition position, MoveDirection direction);
private:
    gsl::not_null<PositionSystem*> _currentPositionManager;
    gsl::not_null<PositionSystem*> _nextPositionManager;
    std::shared_mutex _positionMutex;
};