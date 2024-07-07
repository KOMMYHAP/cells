#include "test_processor_debugger.h"

#include "processor/processor_context.h"

bool TestProcessorDebugger::ShouldAttachDebugger(const ProcessorContext& context) const
{
    return true;
}

void TestProcessorDebugger::AttachDebugger(ProcessorContext& context)
{
    _initialControlBlock = context.GetControlBlock();
}

void TestProcessorDebugger::DetachDebugger(ProcessorContext& context)
{
    _state = context.GetState();
    if (_state == ProcessorState::Good) {
        return;
    }

    if (_shouldAbortOnErrorState) {
        ASSERT_FAIL("Bad error state!");
    }

    context.ModifyControlBlock() = _initialControlBlock;
}

void TestProcessorDebugger::SetAbortOnBadProcessorState(bool value)
{
    _shouldAbortOnErrorState = value;
}