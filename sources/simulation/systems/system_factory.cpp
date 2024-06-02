#include "system_factory.h"

#include "components/component_registry.h"
#include "sequence_system.h"
#include "system_registry.h"

SystemFactory::SystemFactory(SystemRegistry& systemRegistry, ComponentRegistry& componentRegistry)
    : _systemRegistry(&systemRegistry)
    , _componentRegistry(&componentRegistry)
    , _fixedSequenceIds(_componentRegistry->GetCellsCount(), CellId::Invalid)
{
    for (size_t i = 0; i < _componentRegistry->GetCellsCount(); ++i) {
        _fixedSequenceIds[i] = static_cast<CellId>(i);
    }
}

SystemHandle SystemFactory::Make(std::string_view name, const std::span<ComponentHandle>& handles, std::function<void(const SystemContext&)> function)
{
    auto system = std::make_unique<SequenceSystem>(*_componentRegistry, name, handles);
    system->SetSequence(_fixedSequenceIds);
    system->SetFunction(std::move(function));
    return _systemRegistry->Register(std::move(system));
}