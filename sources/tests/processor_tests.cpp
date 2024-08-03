#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "utils/test_processor_debugger.h"
#include "utils/test_processor_state_guard.h"
#include "vm/virtual_machine.h"

namespace {

class ProcessorFixture;

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
        return _debugger.GetLastProcessorState();
    }

    TestProcessorStateGuard MakeScopeWithoutAssert()
    {
        return { _debugger, false, true };
    }

    void Tick()
    {
        _vm->Run({ _memoryBuffer });
    }

protected:
    void SetUp() override
    {
        _vm = MakeVm();
        MakeMemory(255);

        ProcessorMemory rawMemory { _memoryBuffer };
        ProcessorControlBlock controlBlock = {};
        rawMemory.Write(controlBlock);
    }

    void TearDown() override { }

private:
    std::unique_ptr<VirtualMachine> MakeVm()
    {
        auto vm = std::make_unique<VirtualMachine>();
        vm->SetDebugger(&_debugger);
        return vm;
    }

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