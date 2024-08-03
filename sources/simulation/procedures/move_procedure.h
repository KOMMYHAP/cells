#pragma once

#include "procedures/procedure.h"

class CellLocator;

class MoveProcedure final : public ProcedureBase {
public:
    MoveProcedure(EcsWorld& world);

    void Execute(ProcedureContext& context) override;

private:
    gsl::not_null<EcsWorld*> _world;
};