#pragma once

#include "processor/processor_control_block.h"
#include "processor/processor_debugger.h"
#include "processor/processor_state.h"

class TestProcessorDebugger final : public ProcessorDebugger {
public:
    bool ShouldAttachDebugger(const ProcessorContext& context) const override;
    void AttachDebugger(ProcessorContext& context) override;
    void DetachDebugger(ProcessorContext& context) override;

    void ProcedureWillStarted(ProcessorContext& processorContext, ProcedureContext& procedureContext) override;
    void ProcedureWasDeferred(ProcessorContext& processorContext, const ProcedureContext& procedureContext) override;
    void ProcedureWasCompleted(ProcessorContext& processorContext, const ProcedureContext& procedureContext) override;

    bool SetAbortOnBadProcessorState(bool value);
    bool EnableRollbackOnBadProcessorState(bool value);
    
    ProcessorState GetLastProcessorState() const { return _processorState; }
    ProcedureState GetLastProcedureState() const { return _procedureState; }

private:
    ProcessorState _processorState { ProcessorState::Good };
    ProcedureState _procedureState { ProcedureState::Initial };
    ProcessorControlBlock _initialControlBlock {};
    bool _shouldAbortOnErrorState { false };
    bool _shouldRollbackOnErrorState { true };
};