#pragma once

#include "components/cell_id.h"
#include "sequence_system.h"
#include "system_handle.h"

class SystemRegistry {
public:
    SystemRegistry(ComponentRegistry& registry);
    SystemRegistry(const SystemRegistry&) = delete;
    const SystemRegistry& operator=(const SystemRegistry&) = delete;

    SystemHandle Register(std::unique_ptr<SystemBase> system);

    SystemBase& Modify(SystemHandle handle);
    const SystemBase& Get(SystemHandle handle) const;

private:
    ComponentRegistry& _registry;
    std::map<SystemHandle, std::unique_ptr<SystemBase>> _systems;
    SystemHandle _nextHandle { 0 };
};