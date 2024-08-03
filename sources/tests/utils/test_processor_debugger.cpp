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
    _processorState = context.GetState();
    if (_processorState == ProcessorState::Good || context.HasPendingProcedure()) {
        return;
    }

    if (_shouldAbortOnErrorState) {
        ASSERT_FAIL("Bad error state!");
    }

    if (_shouldRollbackOnErrorState) {
        context.ModifyControlBlock() = _initialControlBlock;
    }
}

void TestProcessorDebugger::ProcedureWillStarted(ProcessorContext& processorContext, ProcedureContext& procedureContext)
{
    _procedureState = procedureContext.GetState();
}

void TestProcessorDebugger::ProcedureWasDeferred(ProcessorContext& processorContext, const ProcedureContext& procedureContext)
{
    _procedureState = procedureContext.GetState();
}

void TestProcessorDebugger::ProcedureWasCompleted(ProcessorContext& processorContext, const ProcedureContext& procedureContext)
{
    _procedureState = procedureContext.GetState();
}

bool TestProcessorDebugger::SetAbortOnBadProcessorState(bool value)
{
    return std::exchange(_shouldAbortOnErrorState, value);
}

bool TestProcessorDebugger::EnableRollbackOnBadProcessorState(bool value)
{
    return std::exchange(_shouldRollbackOnErrorState, value);
}