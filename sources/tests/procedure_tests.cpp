#include "procedures/procedure_context.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_stack.h"
#include "utils/test_processor_debugger.h"
#include "utils/test_processor_state_guard.h"
#include "vm/virtual_machine.h"

namespace {

class ProcedureFixture;

class TestProcedure : public ProcedureBase {
public:
    std::function<void(ProcedureContext&)> func;

    void Execute(ProcedureContext& context) override
    {
        func(context);
    }
};

class ProcedureFixture : public testing::Test {
public:
    VirtualMachine* vm { nullptr };

    ProcessorControlBlock& AccessControlBlock()
    {
        ProcessorMemory memory { _memoryBuffer };
        return memory.Access<ProcessorControlBlock>();
    }

    ProcessorMemory GetMemory()
    {
        ProcessorMemory memory { _memoryBuffer };
        memory.Move<ProcessorControlBlock>();
        return memory;
    }

    ProcessorStack GetStack()
    {
        ProcessorControlBlock& controlBlock = AccessControlBlock();
        ProcessorStack stack { controlBlock.stack, controlBlock.stackOffset };
        return stack;
    }

    ProcessorState GetLastProcessorState()
    {
        return _debugger.GetLastProcessorState();
    }

    ProcedureState GetLastProcedureState()
    {
        return _debugger.GetLastProcedureState();
    }

    TestProcessorStateGuard MakeScopeWithoutAssert()
    {
        return { _debugger, false, true };
    }
    TestProcessorStateGuard MakeScopeWithoutRollback()
    {
        return { _debugger, false, false };
    }

    void Tick(ProcessorUserData userData = {})
    {
        _vm->Run({ _memoryBuffer }, userData);
    }

    void CompleteDeferred(const ProcedureContext& context)
    {
        _vm->CompleteDeferredExecution({ _memoryBuffer }, context);
    }

    void TestRollback(ProcessorState expectedProcessorState, ProcedureState expectedProcedureState)
    {
        const auto oldControlBlock = AccessControlBlock();

        auto checkControlBlockWasNotChanged = [&]() {
            ProcessorControlBlock& controlBlock = AccessControlBlock();
            ASSERT_EQ(GetLastProcessorState(), expectedProcessorState);
            ASSERT_EQ(GetLastProcedureState(), expectedProcedureState);
            ASSERT_EQ(controlBlock.nextCommand, oldControlBlock.nextCommand);
            ASSERT_EQ(controlBlock.flags, oldControlBlock.flags);
            ASSERT_EQ(controlBlock.stackOffset, oldControlBlock.stackOffset);
            ASSERT_TRUE(std::equal(controlBlock.stack.begin(), controlBlock.stack.end(), oldControlBlock.stack.begin()));
            ASSERT_TRUE(std::equal(controlBlock.registry.begin(), controlBlock.registry.end(), oldControlBlock.registry.begin()));
        };

        auto _ = MakeScopeWithoutAssert();
        Tick();
        checkControlBlockWasNotChanged();

        Tick();
        checkControlBlockWasNotChanged();
    }

protected:
    void SetUp() override
    {
        _vm = std::make_unique<VirtualMachine>();
        vm = _vm.get();
        vm->SetDebugger(&_debugger);

        MakeMemory(255);

        ProcessorMemory rawMemory { _memoryBuffer };
        ProcessorControlBlock controlBlock = {};
        rawMemory.Write(controlBlock);
    }

    void TearDown() override { }

private:
    void MakeMemory(uint8_t size)
    {
        _memoryBuffer.resize(size);
        std::ranges::fill(_memoryBuffer, std::byte { 0xDD });
    }

    std::vector<std::byte> _memoryBuffer;
    std::unique_ptr<VirtualMachine> _vm;
    TestProcessorDebugger _debugger;
};

}

TEST_F(ProcedureFixture, RegisterProcedure)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [](ProcedureContext& context) {};

    std::set<ProcedureId> oldIds;
    for (uint8_t i { 0 }; i < ProcedureTableLimit; ++i) {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 0);
        ASSERT_NE(id, ProcedureId::Invalid);
        ASSERT_EQ(oldIds.find(id), oldIds.end());
        oldIds.insert(id);
    }
    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 0);
    ASSERT_EQ(id, ProcedureId::Invalid);
    ASSERT_EQ(oldIds.find(id), oldIds.end());
}

TEST_F(ProcedureFixture, UnknownProcedure)
{
    ProcedureId id { 0 };
    GetMemory().Write(ProcessorInstruction::Call, id);
    auto _ = MakeScopeWithoutAssert();
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::UnknownProcedure);
}

TEST_F(ProcedureFixture, CheckProcedureIsCalled)
{
    int counter { 0 };
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        counter += 1;
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 0);
    ASSERT_EQ(counter, 0);

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::Call, id);
    Tick();
    ASSERT_EQ(counter, 1);

    accessor.Write(ProcessorInstruction::Call, id);
    Tick();
    ASSERT_EQ(counter, 2);

    accessor.Write(ProcessorInstruction::Jump, std::byte { 0 });
    Tick();
    ASSERT_EQ(counter, 2);

    Tick();
    ASSERT_EQ(counter, 3);

    Tick();
    ASSERT_EQ(counter, 4);
}

TEST_F(ProcedureFixture, Procedure_In1_Out0)
{
    int counter { 0 };
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        const auto [s, data] = context.TryPopArgs<std::byte>();
        if (!s) {
            return;
        }

        counter += 1;
        ASSERT_EQ(data, std::byte { 42 });
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 0);

    GetMemory().Write(ProcessorInstruction::Call, id);
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
        ASSERT_EQ(counter, 0);
        ASSERT_EQ(GetLastProcessorState(), ProcessorState::AbortedProcedure);
    }

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    accessor.Write(ProcessorInstruction::Call, id);
    accessor.Write(ProcessorInstruction::Jump, std::byte { 0 });
    Tick();
    Tick();
    Tick();
    ASSERT_EQ(counter, 1);
    ASSERT_EQ(GetStack().GetPushedBytesCount(), 0);

    Tick();
    Tick();
    Tick();
    ASSERT_EQ(counter, 2);
}

TEST_F(ProcedureFixture, Procedure_In0_Out1)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        static uint8_t counter { 0 };
        counter += 1;
        const bool s = context.TryPushResult(counter);
        ASSERT_TRUE(s);
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::Call, id);
    {
        Tick();
        const auto [s, data] = GetStack().TryPop<std::byte>();
        ASSERT_TRUE(s);
        ASSERT_EQ(data, std::byte { 1 });
    }

    accessor.Write(ProcessorInstruction::Jump, std::byte { 0 });
    Tick();
}

TEST_F(ProcedureFixture, Procedure_InLimit)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [](ProcedureContext& context) {};
    if constexpr (ProcedureInputArgsCountLimit > 0) {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), ProcedureInputArgsCountLimit - 1, 0);
        ASSERT_NE(id, ProcedureId::Invalid);
    }
    {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), ProcedureInputArgsCountLimit, 0);
        ASSERT_NE(id, ProcedureId::Invalid);
    }
    {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), ProcedureInputArgsCountLimit + 1, 0);
        ASSERT_EQ(id, ProcedureId::Invalid);
    }
}

TEST_F(ProcedureFixture, Procedure_OutLimit)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [](ProcedureContext& context) {};
    if constexpr (ProcedureOutputArgsCountLimit > 0) {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, ProcedureOutputArgsCountLimit - 1);
        ASSERT_NE(id, ProcedureId::Invalid);
    }
    {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, ProcedureOutputArgsCountLimit);
        ASSERT_NE(id, ProcedureId::Invalid);
    }
    {
        const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, ProcedureOutputArgsCountLimit + 1);
        ASSERT_EQ(id, ProcedureId::Invalid);
    }
}

TEST_F(ProcedureFixture, Procedure_In1_Out1)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        const auto [popResult, popData] = context.TryPopArgs<std::byte>();
        ASSERT_TRUE(popResult);
        ASSERT_EQ(popData, std::byte { 42 });
        const bool pushResult = context.TryPushResult(std::byte { 43 });
        ASSERT_TRUE(pushResult);
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 1);

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    accessor.Write(ProcessorInstruction::Call, id);
    Tick();
    Tick();

    const auto [r, data] = GetStack().TryPop<std::byte>();
    ASSERT_TRUE(r);
    ASSERT_EQ(data, std::byte { 43 });
}

TEST_F(ProcedureFixture, AbortProcedure)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        context.AbortProcedure();
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    GetMemory().Write(ProcessorInstruction::Call, id);
    auto _ = MakeScopeWithoutAssert();
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::AbortedProcedure);
}

TEST_F(ProcedureFixture, AbortProcedure_IgnoreUsageAfterAborted_PushArgs)
{
    auto procedure = std::make_unique<TestProcedure>();
    bool outPushed = true;
    procedure->func = [&](ProcedureContext& context) {
        context.AbortProcedure();
        const bool pushed = context.TryPushResult(std::byte { 42 });
        outPushed = pushed;
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    GetMemory().Write(ProcessorInstruction::Call, id);
    auto _ = MakeScopeWithoutAssert();
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::AbortedProcedure);
    ASSERT_EQ(outPushed, false);
}

TEST_F(ProcedureFixture, AbortProcedure_IgnoreUsageAfterAborted_PopArgs)
{
    auto procedure = std::make_unique<TestProcedure>();
    bool outRead = true;
    procedure->func = [&](ProcedureContext& context) {
        context.AbortProcedure();
        const auto [read, _] = context.TryPopArgs<std::byte>();
        outRead = read;
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 0);
    ProcessorMemory memory = GetMemory();
    memory.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    Tick();
    auto _ = MakeScopeWithoutAssert();
    memory.Write(ProcessorInstruction::Call, id);
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::AbortedProcedure);
    ASSERT_EQ(outRead, false);
}

TEST_F(ProcedureFixture, Rollback_InvalidCommandInProcedure)
{
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        context.AbortProcedure();
        context.TryPushResult(std::byte { 42 });
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    GetMemory().Write(ProcessorInstruction::Call, id);
    TestRollback(ProcessorState::AbortedProcedure, ProcedureState::Aborted);
}

TEST_F(ProcedureFixture, Rollback_TooMuchInputRequested)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        const auto [r, data1, data2] = context.TryPopArgs<std::byte, std::byte>();
        ASSERT_FALSE(r);
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 0);
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 1 });
    Tick();
    accessor.Write(ProcessorInstruction::Call, id);
    TestRollback(ProcessorState::AbortedProcedure, ProcedureState::FailedTooMuchInputRequested);
}

TEST_F(ProcedureFixture, Rollback_TooMuchOuput)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {
        const bool r = context.TryPushResult(std::byte { 2 }, std::byte { 3 });
        ASSERT_FALSE(r);
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    accessor.Write(ProcessorInstruction::Call, id);
    TestRollback(ProcessorState::AbortedProcedure, ProcedureState::FailedTooMuchOutput);
}

TEST_F(ProcedureFixture, DeferredExecution_Complete)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    std::optional<ProcedureContext> procedureContext;
    procedure->func = [&](ProcedureContext& context) {
        procedureContext = context;
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    accessor.Write(ProcessorInstruction::Call, id);
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::PendingProcedure);
    ASSERT_TRUE(procedureContext.has_value());

    const bool r = procedureContext->TryPushResult(std::byte { 42 });
    ASSERT_TRUE(r);

    CompleteDeferred(*procedureContext);
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::Good);
}

TEST_F(ProcedureFixture, DeferredExecution_Abort)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    std::optional<ProcedureContext> procedureContext;
    procedure->func = [&](ProcedureContext& context) {
        procedureContext = context;
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    accessor.Write(ProcessorInstruction::Call, id);
    Tick();
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::PendingProcedure);
    ASSERT_TRUE(procedureContext.has_value());

    auto _ = MakeScopeWithoutRollback();
    procedureContext->AbortProcedure();
    CompleteDeferred(*procedureContext);

    ASSERT_EQ(GetLastProcessorState(), ProcessorState::AbortedProcedure);
    ASSERT_EQ(GetLastProcedureState(), ProcedureState::Aborted);
}

TEST_F(ProcedureFixture, UserData)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    struct TestUserData {
        uint32_t value;
    };
    TestUserData outUserData;
    procedure->func = [&](const ProcedureContext& context) {
        outUserData = context.GetUserData().Get<TestUserData>();
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 0);
    accessor.Write(ProcessorInstruction::Call, id);
    Tick(ProcessorUserData(123));
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::Good);
    ASSERT_EQ(outUserData.value, 123);
}

TEST_F(ProcedureFixture, ProcessorStackUpdated)
{
    auto accessor = GetMemory();
    auto sumProcedure = std::make_unique<TestProcedure>();
    sumProcedure->func = [&](ProcedureContext& context) {
        const auto [popped, arg1, arg2] = context.TryPopArgs<uint8_t, uint8_t>();
        ASSERT_TRUE(popped);
        const uint8_t sum = arg1 + arg2;
        context.TryPushResult(sum);
    };
    uint8_t extractedValue = 0;
    auto extractStackProcedure = std::make_unique<TestProcedure>();
    extractStackProcedure->func = [&](ProcedureContext& context) {
        const auto [poped, stackValue] = context.TryPopArgs<uint8_t>();
        ASSERT_TRUE(poped);
        extractedValue = stackValue;
    };

    const ProcedureId sumProcedureId = vm->RegisterProcedure(sumProcedure.get(), 2, 1);
    const ProcedureId extractedProcedureId = vm->RegisterProcedure(extractStackProcedure.get(), 1, 0);
    accessor.Write(ProcessorInstruction::PushStackValue, uint8_t { 10 });
    Tick();
    accessor.Write(ProcessorInstruction::PushStackValue, uint8_t { 20 });
    Tick();
    accessor.Write(ProcessorInstruction::PushStackValue, uint8_t { 30 });
    Tick();
    accessor.Write(ProcessorInstruction::Call, sumProcedureId);
    Tick();
    accessor.Write(ProcessorInstruction::Call, sumProcedureId);
    Tick();
    accessor.Write(ProcessorInstruction::Call, extractedProcedureId);
    Tick();

    ASSERT_EQ(extractedValue, uint8_t { 60 });
}