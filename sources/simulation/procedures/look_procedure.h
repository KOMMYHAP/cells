#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;
class PositionSystem;
class TypeSystem;

class LookProcedure final : public ProcedureBase {
public:
    LookProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, TypeSystem& typeSystem);

    void Execute(ProcedureContext& context) override;

private:
    const SimulationVirtualMachine& _vm;
    PositionSystem& _positionSystem;
    TypeSystem& _typeSystem;
};