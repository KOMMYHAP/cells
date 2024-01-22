#include "vm/virtual_machine.h"
namespace {

class ProcessorMemoryFixture : public testing::Test {
public:
    void MakeMemory(uint8_t size)
    {
        _memoryBuffer.resize(size);
        std::fill(_memoryBuffer.begin(), _memoryBuffer.end(), std::byte { 0xDD });
    }

    ProcessorMemory GetMemory()
    {
        return { _memoryBuffer };
    }

private:
    std::vector<std::byte> _memoryBuffer;
};

}

TEST_F(ProcessorMemoryFixture, Memory_Size)
{
    ASSERT_EQ(GetMemory().Size(), 0);
    MakeMemory(1);
    ASSERT_EQ(GetMemory().Size(), 1);
    ASSERT_TRUE(GetMemory().HasBytes(1));

    auto accessor = GetMemory();
    accessor.Move(1);
    ASSERT_EQ(accessor.Size(), 0);
    ASSERT_FALSE(accessor.HasBytes(1));
}

TEST_F(ProcessorMemoryFixture, Memory_Write_Read)
{
    MakeMemory(1);
    GetMemory().Write(std::byte { 42 });
    const auto data = GetMemory().Read<std::byte>();
    ASSERT_EQ(data, std::byte { 42 });
}

TEST_F(ProcessorMemoryFixture, Memory_2Write_2Read)
{
    MakeMemory(2);
    auto accessor = GetMemory();
    accessor.Write(std::byte { 1 });
    accessor.Write(std::byte { 2 });
    auto view = GetMemory();
    {
        const auto data = view.Read<std::byte>();
        ASSERT_EQ(data, std::byte { 1 });
    }
    {
        const auto data = view.Read<std::byte>();
        ASSERT_EQ(data, std::byte { 2 });
    }
}

TEST_F(ProcessorMemoryFixture, Memory_2Write_Move_Read)
{
    MakeMemory(2);
    {
        auto accessor = GetMemory();
        accessor.Write(std::byte { 1 });
        accessor.Write(std::byte { 2 });
    }
    auto view = GetMemory();
    view.Move<std::byte>();
    {
        const auto data = view.Read<std::byte>();
        ASSERT_EQ(data, std::byte { 2 });
    }
}

TEST_F(ProcessorMemoryFixture, Memory_HasBytes)
{
    MakeMemory(0);
    {
        ASSERT_TRUE(GetMemory().HasBytes(0));
        ASSERT_FALSE(GetMemory().HasBytes(1));
    }
    MakeMemory(1);
    {
        ASSERT_TRUE(GetMemory().HasBytes(1));
        ASSERT_FALSE(GetMemory().HasBytes(2));
    }
    {
        auto accessor = GetMemory();
        accessor.Write(std::byte { 0 });
        ASSERT_FALSE(accessor.HasBytes(1));
    }
    {
        GetMemory().Write(std::byte { 0 });
        ASSERT_TRUE(GetMemory().HasBytes(1));
    }
}

TEST_F(ProcessorMemoryFixture, Memory_TryWrite)
{
    MakeMemory(1);
    {
        ASSERT_TRUE(GetMemory().TryWrite(std::byte { 0 }));
    }
    {
        auto accessor = GetMemory();
        ASSERT_TRUE(accessor.TryWrite(std::byte { 0 }));
        ASSERT_FALSE(accessor.TryWrite(std::byte { 0 }));
    }
    {
        auto accessor = GetMemory();
        accessor.Move<std::byte>();
        ASSERT_FALSE(accessor.TryWrite(std::byte { 0 }));
    }
}

TEST_F(ProcessorMemoryFixture, Memory_TryRead)
{
    MakeMemory(1);
    {
        auto accessor = GetMemory();
        accessor.Write(std::byte { 42 });
    }
    {
        auto accessor = GetMemory();
        auto [result, data] = accessor.TryRead<std::byte>();
        ASSERT_TRUE(result);
        ASSERT_EQ(data, std::byte { 42 });
    }
    {
        auto accessor = GetMemory();
        auto [result, data] = accessor.TryRead<std::byte>();
        ASSERT_TRUE(result);
    }
    {
        auto accessor = GetMemory();
        auto [result1, data1] = accessor.TryRead<std::byte>();
        ASSERT_TRUE(result1);
        auto [result2, data2] = accessor.TryRead<std::byte>();
        ASSERT_FALSE(result2);
    }
    {
        auto accessor = GetMemory();
        accessor.Move<std::byte>();
        auto [result, _] = accessor.TryRead<std::byte>();
        ASSERT_FALSE(result);
    }
}

TEST_F(ProcessorMemoryFixture, Memory_Peek)
{
    MakeMemory(1);
    GetMemory().Write(std::byte { 42 });
    ASSERT_EQ(GetMemory().Peek<std::byte>(), std::byte { 42 });
    ASSERT_EQ(GetMemory().Peek<std::byte>(), std::byte { 42 });
}

TEST_F(ProcessorMemoryFixture, Memory_Access)
{
    MakeMemory(1);
    {
        std::byte& ref = GetMemory().Access<std::byte>();
        ref = std::byte { 42 };
        ASSERT_EQ(GetMemory().Read<std::byte>(), std::byte { 42 });
    }
    {
        std::byte& ref1 = GetMemory().Access<std::byte>();
        std::byte& ref2 = GetMemory().Access<std::byte>();
        ASSERT_EQ(&ref1, &ref2);
    }
    {
        std::byte& ref1 = GetMemory().Access<std::byte>();
        std::byte& ref2 = GetMemory().Access<std::byte>();
        ref1 = std::byte { 42 };
        ASSERT_EQ(ref2, std::byte { 42 });
    }
}

TEST_F(ProcessorMemoryFixture, Memory_WriteStructure)
{
    struct Data {
        uint8_t field1;
        uint16_t field2;
        uint32_t field3;
        std::array<std::byte, 42> buffer;
    };
    MakeMemory(1);
    {
        auto [result, _] = GetMemory().TryRead<Data>();
        ASSERT_FALSE(result);
    }
    {
        ASSERT_FALSE(GetMemory().TryWrite(Data {}));
    }
    {
        ASSERT_FALSE(GetMemory().HasBytes<Data>());
    }

    MakeMemory(sizeof(Data));
    {
        auto [result, _] = GetMemory().TryRead<Data>();
        ASSERT_TRUE(result);
    }
    {
        ASSERT_TRUE(GetMemory().TryWrite(Data {}));
    }
    {
        ASSERT_TRUE(GetMemory().HasBytes<Data>());
    }

    Data data;
    data.field1 = 1;
    data.field2 = 2;
    data.field3 = 3;
    std::fill(data.buffer.begin(), data.buffer.end(), std::byte { 'x' });

    {
        auto accessor = GetMemory();
        ASSERT_TRUE(accessor.TryWrite(data));
        ASSERT_FALSE(accessor.HasBytes(1));
    }
    {
        auto accessor = GetMemory();
        auto [result, readData] = accessor.TryRead<Data>();
        ASSERT_TRUE(result);
        ASSERT_EQ(data.field1, readData.field1);
        ASSERT_EQ(data.field2, readData.field2);
        ASSERT_EQ(data.field3, readData.field3);

        const bool r = std::all_of(data.buffer.begin(), data.buffer.end(), [](std::byte b) { return b == std::byte { 'x' }; });
        ASSERT_TRUE(r);
    }
}

TEST_F(ProcessorMemoryFixture, Memory_Write_DeathTest)
{
    MakeMemory(1);
    auto accessor = GetMemory();
    ASSERT_DEATH(accessor.Write(std::byte { 42 }, std::byte { 42 }), "");
}

TEST_F(ProcessorMemoryFixture, Memory_WriteBig_DeathTest)
{
    MakeMemory(1);
    auto accessor = GetMemory();
    struct Data {
        std::byte buffer[100];
    };
    ASSERT_DEATH(accessor.Write(Data {}), "");
}

TEST_F(ProcessorMemoryFixture, Memory_Move_DeathTest)
{
    MakeMemory(1);
    auto accessor = GetMemory();
    ASSERT_DEATH(accessor.Move(2), "");
}