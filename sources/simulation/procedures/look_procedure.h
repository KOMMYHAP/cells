#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;
class CellLocator;
class TypeSystem;

class LookProcedure final : public ProcedureBase {
public:
    LookProcedure(const SimulationVirtualMachine& vm, CellLocator& positionSystem, TypeSystem& typeSystem);

    void Execute(ProcedureContext& context) override;

private:
    const SimulationVirtualMachine& _vm;
    CellLocator& _positionSystem;
    TypeSystem& _typeSystem;
};