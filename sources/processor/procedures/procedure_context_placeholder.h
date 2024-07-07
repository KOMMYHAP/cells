#pragma once
#include "procedure_context.h"

class PendingProcedurePlaceholder {
public:
    void Set(const ProcedureContext& context);
    ProcedureContext Extract();

private:
    std::unique_ptr<ProcedureContext> _contextPlace;
};