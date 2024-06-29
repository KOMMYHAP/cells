#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "vm/virtual_machine.h"

namespace {

class ProcessorFixture;

class DisabledAssertScope {
public:
    DisabledAssertScope(ProcessorFixture& fixture);
    ~DisabledAssertScope();

private:
    ProcessorFixture& _fixture;
};

class ProcessorFixture : public testing::Test {
public:
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

    void MakeVmWithCustomSystemInstructionPerTick(uint8_t count)
    {
        _vm = MakeVm(count);
    }

protected:
    void SetUp() override
    {
        _vm = MakeVm(1);
        MakeMemory(255);

        ProcessorMemory rawMemory { _memoryBuffer };
        ProcessorControlBlock controlBlock = {};
        rawMemory.Write(controlBlock);
    }

    void TearDown() override { }

private:
    std::unique_ptr<VirtualMachine> MakeVm(uint8_t systemInstructionPerTick)
    {
        auto vm = std::make_unique<VirtualMachine>();
        vm->SetWatcher(MakeKiller());
        vm->SetInstructionsPerStep(systemInstructionPerTick);
        return vm;
    }

    ProcessorStateWatcher MakeKiller()
    {
        return [this](ProcessorState state, ProcessorExternalContext& context) {
            if (_assertOnBadState) {
                ASSERT(state != ProcessorState::Good);
            }
            _lastProcessorState = state;
        };
    }

    void MakeMemory(uint8_t size)
    {
        _memoryBuffer.resize(size);
        std::fill(_memoryBuffer.begin(), _memoryBuffer.end(), std::byte { 0xDD });
    }

    std::vector<std::byte> _memoryBuffer;
    std::unique_ptr<VirtualMachine> _vm;
    bool _assertOnBadState { true };
    ProcessorState _lastProcessorState { ProcessorState::Good };
};

DisabledAssertScope::DisabledAssertScope(ProcessorFixture& fixture)
    : _fixture(fixture)
{
    fixture.EnableAssertsOnBadProcessorState(false);
}

DisabledAssertScope::~DisabledAssertScope()
{
    _fixture.EnableAssertsOnBadProcessorState(true);
}

}

TEST_F(ProcessorFixture, Processor_Init_Term) { }

TEST_F(ProcessorFixture, Processor_Nope)
{
    GetMemory().Write(ProcessorInstruction::Nope);
    Tick();
}

TEST_F(ProcessorFixture, Processor_NextCommandOutOfMemory)
{
    AccessControlBlock().nextCommand = GetMemory().Size();
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
    }
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::OutOfMemory);
}

TEST_F(ProcessorFixture, Processor_StackOverflow)
{
    auto accessor = GetMemory();
    for (uint8_t i = 0; i < ProcessorStackSize; ++i) {
        accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 1 });
    }
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 1 });

    for (uint8_t i = 0; i < ProcessorStackSize; ++i) {
        Tick();
    }
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
    }
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::StackOverflow);
}

TEST_F(ProcessorFixture, Processor_StackUnderflow)
{
    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PopStackRegistry, std::uint8_t { 0 });
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
    }
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::StackUnderflow);
}

TEST_F(ProcessorFixture, Processor_PopStackToInvalidRegister)
{
    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    accessor.Write(ProcessorInstruction::PopStackRegistry, std::uint8_t { ProcessorRegistryCount });

    Tick();
    {
        auto _ = MakeScopeWithoutAssert();
        Tick();
    }
    ASSERT_EQ(GetLastProcessorState(), ProcessorState::InvalidInstruction);
}

TEST_F(ProcessorFixture, Processor_SystemInstructionPerTick)
{
    MakeVmWithCustomSystemInstructionPerTick(2);

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 24 });

    ASSERT_EQ(AccessControlBlock().stackOffset, 0);
    Tick();
    ASSERT_EQ(AccessControlBlock().stackOffset, 2);
}

TEST_F(ProcessorFixture, Processor_SystemInstructionPerTick_WithJump)
{
    MakeVmWithCustomSystemInstructionPerTick(3);

    auto accessor = GetMemory();
    accessor.Write(ProcessorInstruction::PushStackValue, std::byte { 42 });
    accessor.Write(ProcessorInstruction::Jump, std::byte { 0 });
    accessor.Write(ProcessorInstruction::Nope);

    ASSERT_EQ(AccessControlBlock().stackOffset, 0);
    ASSERT_EQ(AccessControlBlock().nextCommand, 0);
    Tick();
    ASSERT_EQ(AccessControlBlock().stackOffset, 2);
    ASSERT_EQ(AccessControlBlock().nextCommand, 2);
}