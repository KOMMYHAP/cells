#include "test_processor_state_guard.h"

#include "test_processor_debugger.h"

TestProcessorStateGuard::TestProcessorStateGuard(TestProcessorDebugger& debugger, bool enableAssert)
    : _debugger(&debugger)
{
    _initialAssertState = _debugger->SetAbortOnBadProcessorState(enableAssert);
}

TestProcessorStateGuard::~TestProcessorStateGuard()
{
    _debugger->SetAbortOnBadProcessorState(_initialAssertState);
}