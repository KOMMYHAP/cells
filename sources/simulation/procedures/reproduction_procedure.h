#pragma once

#include "procedures/procedure.h"

class ReproductionProcedure final : public ProcedureBase {
public:
    ReproductionProcedure(EcsWorld& world);

    void Execute(ProcedureContext& context) override;

private:
    gsl::not_null<EcsWorld*> _world;
};