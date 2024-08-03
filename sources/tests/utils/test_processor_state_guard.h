#pragma once

class TestProcessorDebugger;

class TestProcessorStateGuard {
public:
    TestProcessorStateGuard(TestProcessorDebugger& debugger, bool enableAssert, bool enableRollback);
    ~TestProcessorStateGuard();

    TestProcessorStateGuard(const TestProcessorStateGuard& other) = delete;
    TestProcessorStateGuard(TestProcessorStateGuard&& other) noexcept = delete;
    TestProcessorStateGuard& operator=(const TestProcessorStateGuard& other) = delete;
    TestProcessorStateGuard& operator=(TestProcessorStateGuard&& other) noexcept = delete;

private:
    gsl::not_null<TestProcessorDebugger*> _debugger;
    bool _initialAssertState{false};
    bool _initialRollbackState{false};
};
