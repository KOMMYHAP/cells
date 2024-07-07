#pragma once

#include "procedures/pending_procedure_id.h"
#include "procedures/procedure.h"
#include "procedures/procedure_table.h"
#include "processor/processor_debugger.h"
#include "processor/processor_memory.h"

class VirtualMachine {
public:
    VirtualMachine();

    ProcedureId RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs);
    void SetDebugger(ProcessorDebugger* debugger);
    
    void Run(ProcessorMemory memory, std::any procedureExternalContext = {});
    ProcedureContext RestoreDeferredExecution(ProcessorMemory memory);

private:
    ProcedureContext ExtractProcedureContext(PendingProcedureId id);
    PendingProcedureId AllocatePendingProcedureId();
    ProcedureContext& ModifyProcedureContext(PendingProcedureId id);
    void FreePendingProcedureId(PendingProcedureId id);

    ProcedureTable _procedureTable;
    ProcessorDebugger* _debugger { nullptr };

    PendingProcedureId _nextFreeId { 0 };
    std::stack<PendingProcedureId, std::vector<PendingProcedureId>> _freeIds;
    std::vector<ProcedureContext> _pendingProcedures;
};
