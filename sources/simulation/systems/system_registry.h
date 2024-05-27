#pragma once

#include "components/cell_id.h"
#include "sequence_system.h"
#include "system_handle.h"

class SystemRegistry {
public:
    SystemRegistry(ComponentRegistry& registry);

    SystemHandle MakeSequenceSystem(std::string_view name, const std::span<ComponentHandle>& handles, std::function<void(const SystemContext&)> function);

    SystemBase& Modify(SystemHandle handle);
    const SystemBase& Get(SystemHandle handle) const;

private:
    ComponentRegistry& _registry;
    std::map<SystemHandle, std::unique_ptr<SystemBase>> _systems;
    std::vector<CellId> _fixedSequenceIds;
    SystemHandle _nextHandle { 0 };
};