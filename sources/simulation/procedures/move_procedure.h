#pragma once

#include "procedures/procedure.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"

enum class MoveDirection : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

class MoveProcedure final : public ProcedureBase {
public:
    MoveProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem);

    void Execute(ProcedureContext& context) override;

private:
    CellPosition TryApplyDirection(CellPosition position, MoveDirection direction);

    PositionSystem& _positionSystem;
    const SimulationVirtualMachine& _vm;
};