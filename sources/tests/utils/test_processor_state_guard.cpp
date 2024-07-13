#include "test_processor_state_guard.h"

#include "test_processor_debugger.h"

TestProcessorStateGuard::TestProcessorStateGuard(TestProcessorDebugger& debugger, bool enableAssert, bool enableRollback)
    : _debugger(&debugger)
{
    _initialAssertState = _debugger->SetAbortOnBadProcessorState(enableAssert);
    _initialRollbackState = _debugger->EnableRollbackOnBadProcessorState(enableRollback);
}

TestProcessorStateGuard::~TestProcessorStateGuard()
{
    _debugger->SetAbortOnBadProcessorState(_initialAssertState);
    _debugger->EnableRollbackOnBadProcessorState(_initialRollbackState);
}