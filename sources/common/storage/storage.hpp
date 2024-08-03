#pragma once

#include "asserts/assert.h"

namespace common {

template <class T>
T& Storage::Modify() const
{
    ASSERT(Has<T>(), "Item was not found in storage!");
    Item& item = Modify(typeid(T));
    auto* typedItem = static_cast<impl::StorageItemHolder<T>*>(item.get());
    return typedItem->value;
}

template <class T>
const T& Storage::Get() const
{
    ASSERT(Has<T>(), "Item was not found in storage!");
    const Item& item = Get(typeid(T));
    const auto* typedItem = static_cast<impl::StorageItemHolder<T>*>(item.get());
    return typedItem->value;
}

template <class T, class... Args>
    requires std::constructible_from<T, Args...>
T& Storage::Store(Args&&... args)
{
    ASSERT(!Has<T>(), "Item was already placed in storage!");
    auto item = std::make_unique<impl::StorageItemHolder<T>>(std::forward<Args>(args)...);
    T& value = item->value;
    Store(std::move(item));
    return value;
}

template <class T>
void Storage::Remove()
{
    ASSERT(Has<T>(), "Item was not found in storage!");
    Remove(typeid(T));
}

template <class T>
bool Storage::Has() const
{
    return Has(typeid(T));
}

}