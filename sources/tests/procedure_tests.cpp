#include "procedures/procedure_context.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_stack.h"
#include "vm/virtual_machine.h"

namespace {

class ProcedureFixture;

class DisabledAssertScope {
public:
    DisabledAssertScope(ProcedureFixture& fixture);
    ~DisabledAssertScope();

private:
    ProcedureFixture& _fixture;
};

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
        return _lastProcessorState;
    }

    DisabledAssertScope MakeScopeWithoutAssert()
    {
        return { *this };
    }

    void EnableAssertsOnBadProcessorState(bool value)
    {
        _assertOnBadState = value;
    }

    void Tick()
    {
        _vm->Run({ _memoryBuffer });
    }

    void TestRollback(ProcessorState expectedErrorState)
    {
        const auto oldControlBlock = AccessControlBlock();

        auto checkControlBlockWasNotChanged = [&]() {
            ProcessorControlBlock& controlBlock = AccessControlBlock();
            ASSERT_EQ(GetLastProcessorState(), expectedErrorState);
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

        vm->SetWatcher(MakeKiller());
        vm->SetInstructionsPerStep(1);
        MakeMemory(255);

        ProcessorMemory rawMemory { _memoryBuffer };
        ProcessorControlBlock controlBlock = {};
        rawMemory.Write(controlBlock);
    }

    void TearDown() override { }

private:
    ProcessorStateWatcher MakeKiller()
    {
        return [this](ProcessorState state, ProcessorExternalContext& context) {
            if (_assertOnBadState) {
                ASSERT(state == ProcessorState::Good);
            }
            _lastProcessorState = state;
        };
    }

    void MakeMemory(uint8_t size)
    {
        _memoryBuffer.resize(size);
        std::ranges::fill(_memoryBuffer, std::byte { 0xDD });
    }

    std::vector<std::byte> _memoryBuffer;
    std::unique_ptr<VirtualMachine> _vm;
    std::unique_ptr<TestProcedure> _procedure;
    bool _assertOnBadState { true };
    ProcessorState _lastProcessorState { ProcessorState::Good };
};

DisabledAssertScope::DisabledAssertScope(ProcedureFixture& fixture)
    : _fixture(fixture)
{
    fixture.EnableAssertsOnBadProcessorState(false);
}

DisabledAssertScope::~DisabledAssertScope()
{
    _fixture.EnableAssertsOnBadProcessorState(true);
}

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
        counter += 1;
        const auto [s, data] = context.TryPopArgs<std::byte>();
        ASSERT_TRUE(s);
        ASSERT_EQ(data, std::byte { 42 });
    };

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 0);

    GetMemory().Write(ProcessorInstruction::Call, id);
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
        ASSERT_EQ(counter, 0);
        ASSERT_EQ(GetLastProcessorState(), ProcessorState::ProcedureMissingInput);
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
    TestRollback(ProcessorState::AbortedProcedure);
}

TEST_F(ProcedureFixture, Rollback_MissingOutput)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {};

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 0, 1);
    accessor.Write(ProcessorInstruction::Call, id);
    TestRollback(ProcessorState::ProcedureMissingOutput);
}

TEST_F(ProcedureFixture, Rollback_IgnoredInput)
{
    auto accessor = GetMemory();
    auto procedure = std::make_unique<TestProcedure>();
    procedure->func = [&](ProcedureContext& context) {};

    const ProcedureId id = vm->RegisterProcedure(procedure.get(), 1, 0);
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 1 });
    Tick();
    accessor.Write(ProcessorInstruction::Call, id);
    TestRollback(ProcessorState::ProcedureIgnoreInput);
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
    TestRollback(ProcessorState::ProcedureTooMuchInputRequested);
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
    TestRollback(ProcessorState::ProcedureTooMuchOutput);
}