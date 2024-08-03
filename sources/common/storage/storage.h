#pragma once

#include "impl/impl.h"

namespace common {

class Storage {
public:
    Storage();
    Storage(const Storage& other) = delete;
    Storage(Storage&& other) noexcept = default;
    Storage& operator=(const Storage& other) = delete;
    Storage& operator=(Storage&& other) noexcept = default;
    ~Storage() noexcept;

    template <class T>
    T& Modify() const;

    template <class T>
    const T& Get() const;

    template <class T, class... Args>
        requires std::constructible_from<T, Args...>
    T& Store(Args&&... args);

    template <class T>
    void Remove();

    template <class T>
    bool Has() const;

    size_t Count() const;

    using ItemType = std::type_index;
    using Item = std::unique_ptr<impl::StorageItemHolderBase>;

    Item& Modify(ItemType type) const;
    const Item& Get(ItemType type) const;

    Item& Store(Item item);
    void Remove(ItemType index);

    bool Has(ItemType index) const;

private:
    std::map<ItemType, Item> _items;
};
}

#include "storage.hpp"