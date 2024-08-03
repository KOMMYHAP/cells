#pragma once
#include "processor/processor_context.h"

class ProcessorDebugger {
public:
    virtual bool ShouldAttachDebugger(const ProcessorContext& context) const = 0;
    virtual void AttachDebugger(ProcessorContext& context) = 0;
    virtual void DetachDebugger(ProcessorContext& context) = 0;

    virtual void ProcedureWillStarted(ProcessorContext& processorContext, ProcedureContext& procedureContext) = 0;
    virtual void ProcedureWasDeferred(ProcessorContext& processorContext, const ProcedureContext& procedureContext) = 0;
    virtual void ProcedureWasCompleted(ProcessorContext& processorContext, const ProcedureContext& procedureContext) = 0;

protected:
    ~ProcessorDebugger() = default;
};