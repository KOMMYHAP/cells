#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_table.h"
#include "processor/processor_debugger.h"
#include "processor/processor_memory.h"

class VirtualMachine {
public:
    ProcedureId RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs);
    void SetDebugger(ProcessorDebugger* debugger);

    void Run(ProcessorMemory memory, ProcessorUserData userData = {});
    void CompleteDeferredExecution(ProcessorMemory memory, const ProcedureContext& procedureContext);

private:
    ProcedureTable _procedureTable;
    ProcessorDebugger* _debugger { nullptr };
};
