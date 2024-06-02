#include "component_registry.h"

ComponentRegistry::ComponentRegistry(uint32_t cellsCount)
    : _cellsCount(cellsCount) {}

ComponentHandle ComponentRegistry::Register(const Component& component)
{
    Expects(!_wasFreeze);
    const ComponentHandle handle = _nextHandle;
    Expects(!_storages.contains(handle));
    _storages[handle] = ComponentStorage{ component, _cellsCount };
    _nextHandle = static_cast<ComponentHandle>(static_cast<uint16_t>(_nextHandle) + 1);
    return handle;
}

ComponentStorage& ComponentRegistry::Modify(const ComponentHandle& handle)
{
    Expects(_wasFreeze);
    Expects(_storages.contains(handle));
    return _storages[handle];
}

const ComponentStorage& ComponentRegistry::Get(const ComponentHandle& handle) const
{
    Expects(_wasFreeze);
    auto it = _storages.find(handle);
    Expects(it != _storages.end());
    return it->second;
}

void ComponentRegistry::Freeze()
{
    Expects(!_wasFreeze);
    _wasFreeze = true;
}