#include "storage/storage.h"

TEST(StorageTest, EmptyStorage)
{
    common::Storage storage;
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
}

TEST(StorageTest, TestOneItem)
{
    common::Storage storage;
    storage.Store<int>(42);
    ASSERT_EQ(storage.Count(), 1);
    ASSERT_EQ(storage.Has<int>(), true);
    ASSERT_EQ(storage.Get<int>(), 42);
    storage.Remove<int>();
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
}

TEST(StorageTest, TestTwoItem)
{
    common::Storage storage;
    storage.Store<int>(42);
    storage.Store<float>(3.14f);
    ASSERT_EQ(storage.Count(), 2);
    ASSERT_EQ(storage.Has<int>(), true);
    ASSERT_EQ(storage.Has<float>(), true);
    ASSERT_EQ(storage.Get<int>(), 42);
    ASSERT_FLOAT_EQ(storage.Get<float>(), 3.14f);
    storage.Remove<int>();
    storage.Remove<float>();
    ASSERT_EQ(storage.Count(), 0);
    ASSERT_EQ(storage.Has<int>(), false);
    ASSERT_EQ(storage.Has<float>(), false);
}

TEST(StorageTest, TestAddRemoveAddItem)
{
    common::Storage storage;
    storage.Store<int>(42);
    storage.Remove<int>();
    storage.Store<int>(100);

    ASSERT_EQ(storage.Count(), 1);
    ASSERT_EQ(storage.Get<int>(), 100);

    storage.Remove<int>();
}

TEST(StorageTest, TestMissingAdd_DeathTest)
{
    auto createThenRemove = []() {
        common::Storage storage;
        storage.Remove<int>();
    };
    ASSERT_DEATH(createThenRemove(), "");
}

TEST(StorageTest, TestAddSameItem_DeathTest)
{
    auto addSameItem = []() {
        common::Storage storage;
        storage.Store<int>();
        storage.Store<int>();
    };
    ASSERT_DEATH(addSameItem(), "");
}

TEST(StorageTest, TestMissingRemove_DeathTest)
{
    auto createAddThenDestroy = []() {
        common::Storage storage;
        storage.Store<int>(0);
    };
    ASSERT_DEATH(createAddThenDestroy(), "");
}

TEST(StorageTest, TestMissingItem_DeathTest)
{
    common::Storage storage;
    ASSERT_DEATH(storage.Get<int>(), "");
}

TEST(StorageTest, TestModifyItem)
{
    common::Storage storage;
    storage.Store<int>(42);
    storage.Modify<int>() = 100;
    ASSERT_EQ(storage.Get<int>(), 100);
    storage.Remove<int>();
}

TEST(StorageTest, TestReferenceOnCreatedItem)
{
    {
        common::Storage storage;
        int& item = storage.Store<int>(42);
        ASSERT_EQ(item, 42);
        storage.Remove<int>();
    }
    {
        common::Storage storage;
        int& lhs = storage.Store<int>(42);
        int& rhs = storage.Modify<int>();
        ASSERT_EQ(&lhs, &rhs);
        storage.Remove<int>();
    }
}

TEST(StorageTest, TestMoveOnlyItem)
{
    std::unique_ptr<int> u;
    u = std::make_unique<int>(42);

    using T = std::unique_ptr<int>;
    common::Storage storage;
    auto& p = storage.Store<T>(std::move(u));
    ASSERT_EQ(*p, 42);

    storage.Remove<T>();
}

TEST(StorageTest, TestPointerOnItem)
{
    common::Storage storage;

    {
        int a = 42;
        auto& p = storage.Store<int*>(&a);
        ASSERT_EQ(p, &a);
    }
    {
        float b = 3.14f;
        auto& p = storage.Store<float*>(&b);
        ASSERT_EQ(p, &b);
    }
    {
        double d = 9.75f;
        auto& p = storage.Store<double*>(&d);
        ASSERT_EQ(p, &d);
    }

    storage.Remove<double*>();
    storage.Remove<float*>();
    storage.Remove<int*>();
}
