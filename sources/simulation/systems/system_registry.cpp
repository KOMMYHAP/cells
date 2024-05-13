#include "system_registry.h"

SystemRegistry::SystemRegistry(const ComponentRegistry& registry)
    : _registry(registry)
{
}

SystemHandle SystemRegistry::Register(const std::span<ComponentHandle>& handles)
{
    const SystemHandle handle = _nextHandle;
    ASSERT(!_systems.contains(handle));
    _systems[handle] = System { _registry, handles };
    _nextHandle = static_cast<SystemHandle>(static_cast<uint16_t>(_nextHandle) + 1);
    return handle;
}

System& SystemRegistry::Modify(SystemHandle handle)
{
    ASSERT(_systems.contains(handle));
    return _systems[handle];
}

const System& SystemRegistry::Get(SystemHandle handle) const
{
    ASSERT(_systems.contains(handle));
    return _systems[handle];
}
