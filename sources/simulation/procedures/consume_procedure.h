#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;
class CellLocator;
class HealthSystem;
class TypeSystem;

class ConsumeProcedure final : public ProcedureBase {
public:
    ConsumeProcedure(const SimulationVirtualMachine& vm, CellLocator& positionSystem, HealthSystem& healthSystem, TypeSystem& typeSystem);

    void Execute(ProcedureContext& context) override;

private:
    const SimulationVirtualMachine& _vm;
    CellLocator& _positionSystem;
    HealthSystem& _healthSystem;
    TypeSystem& _typeSystem;
};