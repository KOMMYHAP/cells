#include "storage.h"
#include "boost/any/unique_any.hpp"

namespace common {
    

static bool StorageItemLess(const Storage::Item & item, Storage::ItemType index)
{
    return Storage::ItemType { item.type() } < index;
};
static bool StorageItemLess(Storage::ItemType index, const Storage::Item & item)
{
    return index < Storage::ItemType { item.type() };
};

Storage::Storage() = default;

Storage::~Storage()
{
    for (auto&& [type, item] : _items) {
        PANIC("You should manually remove all items from storage to ensure correct order!", item);
    }
}

Storage::Item& Storage::Modify(Storage::ItemType type) const
{
    ASSERT(_items.contains(type));
    auto it = _items.find(type);
    return const_cast<Item&>(it->second);
}

const Storage::Item& Storage::Get(Storage::ItemType type) const
{
    ASSERT(_items.contains(type));
    auto it = _items.find(type);
    return it->second;
}

Storage::Item& Storage::Store(Storage::Item item)
{
    const Storage::ItemType type { item.type() };
    ASSERT(!_items.contains(type));
    auto [it, _] = _items.emplace(type, std::move(item));
    return it->second;
}

void Storage::Remove(Storage::ItemType type)
{
    ASSERT(_items.contains(type));
    _items.erase(type);
}

size_t Storage::Count() const
{
    return _items.size();
}

bool Storage::Has(Storage::ItemType type) const
{
    return _items.contains(type);
}

}