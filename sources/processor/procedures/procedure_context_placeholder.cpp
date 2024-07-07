#include "procedure_context_placeholder.h"

void PendingProcedurePlaceholder::Set(const ProcedureContext& context)
{
    if (!_contextPlace) {
        _contextPlace = std::make_unique<ProcedureContext>(context);
        return;
    }

    // reuse space, avoid allocation
    *_contextPlace = context;
}

ProcedureContext PendingProcedurePlaceholder::Extract()
{
    return *_contextPlace;
}