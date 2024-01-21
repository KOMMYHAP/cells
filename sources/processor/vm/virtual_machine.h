#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_table.h"
#include "processor/processor_memory.h"
#include "processor/processor_state.h"

class VirtualMachine {
public:
    VirtualMachine(ProcessorStateWatcher processorStateWatcher, uint8_t systemInstructionPerStep);

    ProcedureId RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs);

    void Run(ProcessorMemory memory);

private:
    ProcedureTable _procedureTable;
    uint8_t _systemInstructionPerStep;
    ProcessorStateWatcher _processorStateWatcher;
};