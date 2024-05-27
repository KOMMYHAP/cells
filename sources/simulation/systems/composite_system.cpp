#include "composite_system.h"

CompositeSystem::CompositeSystem(
    ComponentRegistry& registry,
    std::string_view name,
    const std::span<ComponentHandle>& filterComponents,
    const std::span<ComponentHandle>& resultComponents)
    : SystemBase(registry, name, resultComponents)
    , _filterSystemName(std::format("%s_filter", name))
    , _filterSystem(registry, _filterSystemName, filterComponents)
{
    _filterSystem.SetFunction([this](const SystemContext& context) {
        const bool shouldSkip = _filterFunction(context);
        if (shouldSkip) {
            return;
        }

        _filteredIds.push_back(context.id);
    });
}

void CompositeSystem::Process()
{
    // request vector<CellId> from pool?
    _filteredIds.clear();

    _filterSystem.Process();
    if (_filteredIds.empty()) {
        return;
    }

    ProcessImpl(_filteredIds, _resultFunction);
}
