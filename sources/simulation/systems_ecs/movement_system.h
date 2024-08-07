﻿#pragma once
#include "simulation_ecs_system.h"

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "components/move_direction.h"
#include "simulation/cell_locator.h"

class ICellFactory;

class MovementSystem final : public SimulationEcsSystem<MovementSystem, CellPosition, const MoveDirection> {
public:
    MovementSystem(EcsWorld& ecsWorld, CellLocator& currentPositions);

    void DoProcessComponents(CellId id, CellPosition& position, MoveDirection direction);

private:
    gsl::not_null<CellLocator*> _cellsLocator;
};