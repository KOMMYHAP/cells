#include "test_processor_state_guard.h"

#include "test_processor_debugger.h"

TestProcessorStateGuard::TestProcessorStateGuard(TestProcessorDebugger& debugger)
    : _debugger(&debugger)
{
    _debugger->SetAbortOnBadProcessorState(true);
}

TestProcessorStateGuard::~TestProcessorStateGuard()
{
    _debugger->SetAbortOnBadProcessorState(false);
}