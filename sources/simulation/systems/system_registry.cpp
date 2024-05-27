#include "system_registry.h"
#include "sequence_system.h"

SystemRegistry::SystemRegistry(ComponentRegistry& registry)
    : _registry(registry)
    , _fixedSequenceIds(registry.GetCellsCount(), CellId::Invalid)
{
    for (size_t i = 0; i < registry.GetCellsCount(); ++i) {
        _fixedSequenceIds[i] = static_cast<CellId>(i);
    }
}

SystemHandle SystemRegistry::MakeSequenceSystem(std::string_view name, const std::span<ComponentHandle>& handles, std::function<void(const SystemContext&)> function)
{
    const SystemHandle handle = _nextHandle;
    ASSERT(!_systems.contains(handle));
    auto system = std::make_unique<SequenceSystem>(_registry, name, handles);
    system->SetSequence(_fixedSequenceIds);
    system->SetFunction(std::move(function));
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
