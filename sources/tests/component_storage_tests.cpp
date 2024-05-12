#include "components/component_storage.h"

TEST(ComponentStorageTest, SimpleTestData)
{
    struct TestData {
        int value;
    };

    Component info { 0, sizeof(TestData) };
    ComponentStorage storage { info, 100 };

    ASSERT_EQ(storage.Count(), 100);
    ASSERT_EQ(storage.CountBytes(), 100 * sizeof(TestData));
    ASSERT_EQ(storage.GetMetaInfo().sizeInBytes, info.sizeInBytes);
    ASSERT_EQ(storage.GetMetaInfo().id, info.id);

    TestData& data = storage.Modify<TestData>(0);
    data.value = 42;
    ASSERT_EQ(storage.Get<TestData>(0).value, 42);

    const TestData* p1 = &storage.Get<TestData>(10);
    const TestData* p2 = &storage.Get<TestData>(11);
    ASSERT_EQ(p1 + 1, p2);

    ASSERT_DEATH(storage.Get<TestData>(100), "");
    ASSERT_DEATH(storage.Modify<TestData>(100), "");
    ASSERT_DEATH(storage.Get<double>(0), "");
}
