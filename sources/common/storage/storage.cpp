#include "storage/storage.h"
#include "asserts/assert.h"

namespace common {

Storage::Storage() = default;

Storage::~Storage() noexcept
{
    ASSERT(_items.empty(), "You should manually remove all items from storage to ensure correct order!");
}

Storage::Item& Storage::Modify(ItemType type) const
{
    ASSERT(_items.contains(type), "Item was not found in storage!");
    auto it = _items.find(type);
    return const_cast<Item&>(it->second);
}

const Storage::Item& Storage::Get(ItemType type) const
{
    ASSERT(_items.contains(type), "Item was not found in storage!");
    auto it = _items.find(type);
    return it->second;
}

Storage::Item& Storage::Store(Item item)
{
    const ItemType type { item->GetTypeIndex() };
    ASSERT(!_items.contains(type), "Item was already placed in storage!");
    auto [it, _] = _items.emplace(type, std::move(item));
    return it->second;
}

void Storage::Remove(ItemType type)
{
    ASSERT(_items.contains(type), "Item was not found in storage!");
    _items.erase(type);
}

size_t Storage::Count() const
{
    return _items.size();
}

bool Storage::Has(ItemType type) const
{
    return _items.contains(type);
}

}