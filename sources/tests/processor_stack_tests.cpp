#include "processor/processor_stack.h"
#include "vm/virtual_machine.h"

namespace {

class ProcessorStackFixture : public testing::Test {
public:
    ProcessorStack GetStack()
    {
        return ProcessorStack { _controlBlock };
    }

    void UpdateControlBlock(const ProcessorStack& stack)
    {
        stack.CopyTo(_controlBlock);
    }

protected:
    void SetUp() override
    {
        static_assert(ProcessorStackSize > 0);
        _controlBlock.stackOffset = 0;
        std::ranges::fill(_controlBlock.stack, std::byte { 0xDD });
    }

    void TearDown() override
    {
    }

private:
    ProcessorControlBlock _controlBlock {};
};

}

TEST_F(ProcessorStackFixture, Stack_Empty)
{
    ASSERT_TRUE(GetStack().CanPush<std::byte>());
    ASSERT_FALSE(GetStack().CanPop<std::byte>());
}

TEST_F(ProcessorStackFixture, Stack_Push)
{
    ASSERT_TRUE(GetStack().CanPush<std::byte>());
    ASSERT_TRUE(GetStack().TryPush(std::byte { 42 }));
}

TEST_F(ProcessorStackFixture, Stack_Pop)
{
    {
        ProcessorStack stack = GetStack();
        stack.Push(uint8_t { 42 });
        ASSERT_TRUE(stack.CanPop<uint8_t>());
        ASSERT_EQ(stack.Pop<uint8_t>(), uint8_t { 42 });
    }
    {
        ProcessorStack stack = GetStack();
        stack.Push(uint8_t { 33 });
        const auto [popped, value] = stack.TryPop<uint8_t>();
        ASSERT_TRUE(popped);
        ASSERT_EQ(value, uint8_t { 33 });
    }
}
TEST_F(ProcessorStackFixture, Stack_ControlBlockUpdate)
{
    {
        static_assert(ProcessorStackSize >= 2);
        ProcessorStack stack = GetStack();
        stack.Push(uint8_t { 20 });
        stack.Push(uint8_t { 10 });
        UpdateControlBlock(stack);
    }
    {
        ProcessorStack stack = GetStack();
        ASSERT_TRUE(stack.CanPop<uint8_t>());
        ASSERT_EQ(stack.Pop<uint8_t>(), 10);
        ASSERT_TRUE(stack.CanPop<uint8_t>());
        ASSERT_EQ(stack.Pop<uint8_t>(), 20);
        UpdateControlBlock(stack);
    }
    {
        const ProcessorStack stack = GetStack();
        ASSERT_FALSE(stack.CanPop<uint8_t>());
        ASSERT_TRUE(stack.CanPush<uint8_t>());
    }
}