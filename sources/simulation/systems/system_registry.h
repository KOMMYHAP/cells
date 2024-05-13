#pragma once

#include "system.h"
#include "system_handle.h"

class SystemRegistry {
public:
    SystemRegistry(const ComponentRegistry& registry);
    SystemHandle Register(const std::span<ComponentHandle>& handles);

    System& Modify(SystemHandle handle);
    const System& Get(SystemHandle handle) const;

private:
    const ComponentRegistry& _registry;
    std::map<SystemHandle, System> _systems;
    SystemHandle _nextHandle { 0 };
};