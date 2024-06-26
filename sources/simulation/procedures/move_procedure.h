#pragma once

#include "procedures/procedure.h"

class PositionSystem;
class SimulationVirtualMachine;

class MoveProcedure final : public ProcedureBase {
public:
    MoveProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem);

    void Execute(ProcedureContext& context) override;

private:
    PositionSystem& _positionSystem;
    const SimulationVirtualMachine& _vm;
};