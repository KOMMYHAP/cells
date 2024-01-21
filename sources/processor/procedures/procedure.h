#pragma once

#include "processor/memory.h"
#include "processor/processor_control_block.h"

struct ProcedureInfo;
class ProcessorContext;

class ProcedureContext {
public:
    ProcedureContext(ProcessorContext& context, const Memory& memory);

    const Memory& GetMemory() const { return _memory; }
    Memory& AccessMemory() { return _memory; }

    void SetState(ProcessorState state);

private:
    ProcessorContext& _processorContext;
    Memory _memory;
};

struct ProcedureBase {
    virtual void Execute(ProcedureContext& context) = 0;
};