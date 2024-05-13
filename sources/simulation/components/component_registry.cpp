#include "component_registry.h"

ComponentRegistry::ComponentRegistry(uint32_t cellsCount)
    : _cellsCount(cellsCount)
{
}

ComponentHandle ComponentRegistry::Register(const Component& component)
{
    ASSERT(!_wasFreeze);
    const ComponentHandle handle = _nextHandle;
    ASSERT(!_storages.contains(handle));
    _storages[handle] = ComponentStorage { component, _cellsCount };
    _nextHandle = static_cast<ComponentHandle>(static_cast<uint16_t>(_nextHandle) + 1);
    return handle;
}

ComponentStorage& ComponentRegistry::Modify(const ComponentHandle& handle)
{
    ASSERT(_wasFreeze);
    ASSERT(_storages.contains(handle));
    return _storages[handle];
}

const ComponentStorage& ComponentRegistry::Get(const ComponentHandle& handle) const
{
    ASSERT(_wasFreeze);
    auto it = _storages.find(handle);
    ASSERT(it != _storages.end());
    return it->second;
}

void ComponentRegistry::Freeze()
{
    ASSERT(!_wasFreeze);
    _wasFreeze = true;
}
