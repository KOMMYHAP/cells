#include "component_registry.h"

ComponentRegistry::ComponentRegistry(size_t storageSize)
    : _storageSize(storageSize)
{
}

ComponentHandle ComponentRegistry::Register(const Component& component)
{
    ASSERT(!_wasFreeze);
    const ComponentHandle handle = _nextHandle;
    ASSERT(!_storages.contains(handle));
    _storages[handle] = ComponentStorage { component, _storageSize };
    _nextHandle.id += 1;
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
