#include "storage/stack_storage.h"

TEST(StackStorageTest, EmptyStorage)
{
    common::StackStorage storage;
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
}

TEST(StackStorageTest, TestOneItem)
{
    common::StackStorage storage;
    storage.Store<int>(42);
    ASSERT_EQ(storage.Count(), 1);
    ASSERT_EQ(storage.Has<int>(), true);
    ASSERT_EQ(storage.Get<int>(), 42);
    storage.Remove<int>();
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
}

TEST(StackStorageTest, WrontRemovingOrder_DeathTest)
{
    common::StackStorage storage;
    storage.Store<int>(42);
    storage.Store<float>(3.14f);
    ASSERT_DEATH(storage.Remove<int>(), "");
}

TEST(StackStorageTest, TestTwoItem)
{
    common::StackStorage storage;
    storage.Store<int>(42);
    storage.Store<float>(3.14f);
    ASSERT_EQ(storage.Count(), 2);
    ASSERT_EQ(storage.Has<int>(), true);
    ASSERT_EQ(storage.Has<float>(), true);
    ASSERT_EQ(storage.Get<int>(), 42);
    ASSERT_FLOAT_EQ(storage.Get<float>(), 3.14f);
    storage.Remove<float>();
    storage.Remove<int>();
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
    ASSERT_EQ(storage.Has<float>(), false);
}

TEST(StackStorageTest, TestAddRemoveAddItem)
{
    common::StackStorage storage;
    storage.Store<int>(42);
    storage.Remove<int>();
    storage.Store<int>(100);

    ASSERT_EQ(storage.Count(), 1);
    ASSERT_EQ(storage.Get<int>(), 100);

    storage.Remove<int>();
}

TEST(StackStorageTest, TestMissingAdd_DeathTest)
{
    auto createThenRemove = []() {
        common::StackStorage storage;
        storage.Remove<int>();
    };
    ASSERT_DEATH(createThenRemove(), "");
}

TEST(StackStorageTest, TestAddSameItem_DeathTest)
{
    auto addSameItem = []() {
        common::StackStorage storage;
        storage.Store<int>();
        storage.Store<int>();
    };
    ASSERT_DEATH(addSameItem(), "");
}

TEST(StackStorageTest, TestAutoRemove)
{
    common::StackStorage storage;
    storage.Store<int>(42);
}

TEST(StackStorageTest, TestMissingItem_DeathTest)
{
    common::StackStorage storage;
    ASSERT_DEATH(storage.Get<int>(), "");
}

TEST(StackStorageTest, TestModifyItem)
{
    common::StackStorage storage;
    storage.Store<int>(42);
    storage.Modify<int>() = 100;
    ASSERT_EQ(storage.Get<int>(), 100);
}

TEST(StackStorageTest, TestReferenceOnCreatedItem)
{
    {
        common::StackStorage storage;
        int& item = storage.Store<int>(42);
        ASSERT_EQ(item, 42);
    }
    {
        common::StackStorage storage;
        int& lhs = storage.Store<int>(42);
        int& rhs = storage.Modify<int>();
        ASSERT_EQ(&lhs, &rhs);
    }
}

TEST(StackStorageTest, TestMoveOnlyItem)
{
    auto u = std::make_unique<int>(42);

    using T = std::unique_ptr<int>;
    common::StackStorage storage;
    auto& p = storage.Store<T>(std::move(u));
    ASSERT_EQ(*p, 42);
}
