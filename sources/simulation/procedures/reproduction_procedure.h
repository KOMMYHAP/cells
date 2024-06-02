#pragma once

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "components/cell_position.h"
#include "procedures/procedure.h"

class SimulationVirtualMachine;
class PositionSystem;
class HealthSystem;
class TypeSystem;
class BrainSystem;
class Spawner;

class ReproductionProcedure final : public ProcedureBase {
public:
    ReproductionProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, HealthSystem& healthSystem, BrainSystem& brainSystem, TypeSystem& typeSystem, Spawner& spawner);

    void Execute(ProcedureContext& context) override;

private:
    CellPosition SelectPosition(CellPosition lhs, CellPosition rhs) const;
    CellBrain MakeChildBrain(CellId lhs, CellId rhs) const;

    const SimulationVirtualMachine& _vm;
    PositionSystem& _positionSystem;
    HealthSystem& _healthSystem;
    BrainSystem& _brainSystem;
    TypeSystem& _typeSystem;
    Spawner& _spawner;
};