#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_table.h"
#include "processor/processor_memory.h"
#include "processor/processor_state.h"

class VirtualMachine {
public:
    ProcedureId RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs);
    void SetWatcher(ProcessorStateWatcher watcher);
    void SetInstructionsPerStep(uint8_t count);

    void Run(ProcessorMemory memory);

private:
    ProcedureTable _procedureTable;
    uint8_t _systemInstructionPerStep;
    ProcessorStateWatcher _processorStateWatcher;
};