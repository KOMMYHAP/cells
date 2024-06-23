#pragma once
#include "components/component_handle.h"
#include "system_handle.h"
#include "systems/system_context.h"

class SystemRegistry;
class ComponentRegistry;

class SystemFactory {
public:
    SystemFactory(SystemRegistry& systemRegistry, ComponentRegistry& componentRegistry);
    SystemHandle Make(std::string_view name, std::span<const ComponentHandle> handles, std::function<void(const SystemContext&)> function);

private:
    gsl::not_null<SystemRegistry*> _systemRegistry;
    gsl::not_null<ComponentRegistry*> _componentRegistry;
    std::vector<CellId> _fixedSequenceIds;
};