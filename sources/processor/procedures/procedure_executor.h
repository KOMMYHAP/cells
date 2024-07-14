#pragma once
#include "procedure_context.h"
#include "processor/processor_context.h"

class ProcedureExecutor {
public:
    ProcedureExecutor(ProcessorContext& processorContext, ProcedureContext& procedureContext);

private:
    gsl::not_null<ProcessorContext*> _processorContext;
    gsl::not_null<ProcedureContext*> _procedureContext;
};