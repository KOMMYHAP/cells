#include "components/component_storage.h"

TEST(ComponentStorageTest, SimpleTestData)
{
    struct TestData {
        int value;
    };

    Component info { sizeof(TestData) };
    ComponentStorage storage { info, 100 };

    ASSERT_EQ(storage.Count(), 100);
    ASSERT_EQ(storage.CountBytes(), 100 * sizeof(TestData));
    ASSERT_EQ(storage.GetMetaInfo().sizeInBytes, info.sizeInBytes);

    TestData& data = storage.Modify<TestData>(0);
    data.value = 42;
    ASSERT_EQ(storage.Get<TestData>(0).value, 42);
}

TEST(ComponentStorageTest, Foreach)
{
    struct TestData {
        int value;
    };

    Component info { sizeof(TestData) };
    ComponentStorage storage { info, 100 };

    storage.Foreach<TestData>([](TestData& item) {
        item.value = 0;
    });

    storage.Foreach<TestData>([](const TestData& item) {
        ASSERT_EQ(item.value, 0);
    });
}

TEST(ComponentStorageTest, ForeachInEmptyStorage)
{
    Component info { sizeof(int) };
    ComponentStorage storage { info, 0 };

    storage.Foreach<int>([](int& item) {
        item = 0;
    });
}

TEST(ComponentStorageTest, SequenceOrder)
{
    Component info { sizeof(int) };
    ComponentStorage storage { info, 10 };

    for (size_t i = 0; i < storage.Count() - 1; ++i) {
        const int* p1 = &storage.Get<int>(i);
        const int* p2 = &storage.Get<int>(i + 1);
        ASSERT_EQ(p1 + 1, p2);
    }
}

TEST(ComponentStorageTest, OutOfIndex_DeathTest)
{
    struct TestData {
        int value;
    };

    Component info { sizeof(TestData) };
    ComponentStorage storage { info, 100 };

    EXPECT_DEATH(storage.Get<TestData>(100), "");
    EXPECT_DEATH(storage.Modify<TestData>(100), "");
    EXPECT_DEATH(storage.Get<double>(0), "");
}
