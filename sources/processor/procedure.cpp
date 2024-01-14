#include "procedure.h"
#include "procedure_table.h"
#include "processor.h"

ProcedureContext::ProcedureContext(ProcessorContext& context, const Memory& memory)
    : _processorContext(context)
    , _memory(memory)
{
}

void ProcedureContext::MarkInvalid()
{
    _processorContext.SetFlag(ProcessorFlags::InvalidCommand);
}
