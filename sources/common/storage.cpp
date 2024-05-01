#include "storage.h"

namespace common {

static bool StorageItemLess(const std::any& item, std::type_index index)
{
    return std::type_index { item.type() } < index;
};
static bool StorageItemLess(std::type_index index, const std::any& item)
{
    return index < std::type_index { item.type() };
};

Storage::Storage() = default;

Storage::~Storage()
{
    for (const std::any& item : _items) {
        PANIC("You should manually remove all items from storage to ensure correct order!", item);
    }
}

std::any& Storage::Modify(std::type_index index)
{
    ASSERT(_items.contains(index));
    return _items[index];
}

const std::any& Storage::Get(std::type_index index) const
{
    ASSERT(_items.contains(index));
    auto it = _items.find(index);
    return it->second;
}

void Storage::Store(std::any item)
{
    const std::type_index itemIndex { item.type() };
    ASSERT(!_items.contains(itemIndex));
    _items[itemIndex] = std::move(item);
}

void Storage::Remove(std::type_index index)
{
    ASSERT(_items.contains(index));
    _items.erase(index);
}

size_t Storage::Count() const
{
    return _items.size();
}

bool Storage::Has(std::type_index index) const
{
    return _items.contains(index);
}

}