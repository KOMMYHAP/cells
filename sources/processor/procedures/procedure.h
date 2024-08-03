#pragma once

#include "procedures/procedure_context.h"

class ProcedureBase {
public:
    virtual ~ProcedureBase() = default;
    virtual void Execute(ProcedureContext& context) = 0;
};
