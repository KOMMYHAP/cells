#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;
class PositionSystem;
class HealthSystem;
class TypeSystem;

class ConsumeProcedure final : public ProcedureBase {
public:
    ConsumeProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, HealthSystem& healthSystem, TypeSystem& typeSystem);

    void Execute(ProcedureContext& context) override;

private:
    const SimulationVirtualMachine& _vm;
    PositionSystem& _positionSystem;
    HealthSystem& _healthSystem;
    TypeSystem& _typeSystem;
};