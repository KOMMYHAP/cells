#include "system_executor.h"

#include "system_base.h"
#include "system_registry.h"

SystemExecutor::SystemExecutor(SystemRegistry& registry, std::span<SystemHandle> handles)
{
    _systems.reserve(handles.size());
    for (const SystemHandle handle : handles) {
        SystemBase& system = registry.Modify(handle);
        _systems.push_back(&system);
    }
}

void SystemExecutor::Process()
{
    for (SystemBase* system : _systems) {
        system->ExecutorProcess(_cells);
    }
}