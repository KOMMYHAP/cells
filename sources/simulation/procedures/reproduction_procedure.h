#pragma once

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "components/cell_position.h"
#include "procedures/procedure.h"

class SimulationVirtualMachine;
class CellLocator;
class HealthSystem;
class TypeSystem;
class BrainSystem;
class Spawner;

class ReproductionProcedure final : public ProcedureBase {
public:
    ReproductionProcedure(EcsWorld& world);

    void Execute(ProcedureContext& context) override;

private:
    CellPosition SelectPosition(CellPosition lhs, CellPosition rhs) const;
    CellBrain MakeChildBrain(CellId lhs, CellId rhs) const;

    // gsl::not_null<const SimulationVirtualMachine*> _vm;
    // gsl::not_null<CellLocator*> _locator;
    gsl::not_null<EcsWorld*> _world;
};