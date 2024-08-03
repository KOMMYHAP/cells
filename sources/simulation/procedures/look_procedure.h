#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;
class CellLocator;
class TypeSystem;

class LookProcedure final : public ProcedureBase {
public:
    LookProcedure(EcsWorld& world, const CellLocator& locator);

    void Execute(ProcedureContext& procedureContext) override;

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<const CellLocator*> _locator;
};