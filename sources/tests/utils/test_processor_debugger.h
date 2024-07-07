#pragma once

#include "procedures/procedure_context.h"
#include "processor/processor_control_block.h"
#include "processor/processor_debugger.h"

class TestProcessorDebugger final : public ProcessorDebugger {
public:
    bool ShouldAttachDebugger(const ProcessorContext& context) const override;
    void AttachDebugger(ProcessorContext& context) override;
    void DetachDebugger(ProcessorContext& context) override;

    void SetAbortOnBadProcessorState(bool value);
    ProcessorState GetLastProcessorState() const { return _state; }

private:
    ProcessorState _state{ProcessorState::Good};
    ProcessorControlBlock _initialControlBlock {};
    bool _shouldAbortOnErrorState { false };
};