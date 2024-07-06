#include "processor/processor_stack.h"
#include "vm/virtual_machine.h"

namespace {

class ProcessorStackFixture : public testing::Test {
public:
    void MakeStack(uint8_t size)
    {
        _offset = 0;
        _memoryBuffer.resize(size);
        std::ranges::fill(_memoryBuffer, std::byte { 0xDD });
    }

    ProcessorStack GetStack()
    {
        return { _memoryBuffer, _offset };
    }

private:
    uint8_t _offset { 0 };
    std::vector<std::byte> _memoryBuffer;
};

}

TEST_F(ProcessorStackFixture, Stack_Empty)
{
    ASSERT_FALSE(GetStack().CanPush<std::byte>());
    ASSERT_FALSE(GetStack().CanPop<std::byte>());
}

TEST_F(ProcessorStackFixture, Stack_Push)
{
    MakeStack(1);
    ASSERT_TRUE(GetStack().CanPush<std::byte>());
    ASSERT_TRUE(GetStack().TryPush(std::byte { 42 }));
    ASSERT_FALSE(GetStack().TryPush(std::byte { 42 }));
}

TEST_F(ProcessorStackFixture, Stack_Pop)
{
    MakeStack(1);
    {
        ASSERT_FALSE(GetStack().CanPop<std::byte>());
        auto [success, data] = GetStack().TryPop<std::byte>();
        ASSERT_FALSE(success);
    }
    GetStack().Push(std::byte { 42 });
    {
        ASSERT_TRUE(GetStack().CanPop<std::byte>());
        auto [success, data] = GetStack().TryPop<std::byte>();
        ASSERT_TRUE(success);
        ASSERT_EQ(data, std::byte { 42 });
    }
}

TEST_F(ProcessorStackFixture, Stack_PushPop)
{
    MakeStack(3);
    {
        ASSERT_FALSE(GetStack().CanPop<std::byte>());
    }
    {
        ASSERT_TRUE(GetStack().CanPush<std::byte>());
        GetStack().Push(std::byte { 1 });
        ASSERT_TRUE(GetStack().CanPush<std::byte>());
        GetStack().Push(std::byte { 10 });
        ASSERT_TRUE(GetStack().CanPush<std::byte>());
        GetStack().Push(std::byte { 100 });
        ASSERT_FALSE(GetStack().CanPush<std::byte>());
    }
    {
        ASSERT_TRUE(GetStack().CanPop<std::byte>());
        ASSERT_EQ(GetStack().Pop<std::byte>(), std::byte { 100 });
        ASSERT_TRUE(GetStack().CanPop<std::byte>());
        ASSERT_EQ(GetStack().Pop<std::byte>(), std::byte { 10 });
        ASSERT_TRUE(GetStack().CanPop<std::byte>());
        ASSERT_EQ(GetStack().Pop<std::byte>(), std::byte { 1 });
        ASSERT_FALSE(GetStack().CanPop<std::byte>());
    }
    ASSERT_TRUE(GetStack().CanPush<std::byte>());
}
