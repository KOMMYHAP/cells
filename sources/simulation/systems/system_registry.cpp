#include "system_registry.h"
#include "sequence_system.h"

SystemRegistry::SystemRegistry(ComponentRegistry& registry)
    : _registry(registry) {}

SystemHandle SystemRegistry::Register(std::unique_ptr<SystemBase> system)
{
    const SystemHandle handle = _nextHandle;
    ASSERT(!_systems.contains(handle));
    _systems[handle] = std::move(system);
    _nextHandle = static_cast<SystemHandle>(static_cast<uint16_t>(_nextHandle) + 1);
    return handle;
}

SystemBase& SystemRegistry::Modify(SystemHandle handle)
{
    ASSERT(_systems.contains(handle));
    return *(_systems[handle]);
}

const SystemBase& SystemRegistry::Get(SystemHandle handle) const
{
    ASSERT(_systems.contains(handle));
    auto it = _systems.find(handle);
    return *(it->second);
}