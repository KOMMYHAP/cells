#pragma once

#include "systems/sequence_system.h"
#include "systems/system_base.h"

#include "components/component_handle.h"
#include "components/component_registry.h"

class ComponentStorage;
class ComponentRegistry;

class CompositeSystem final : public SystemBase {
public:
    CompositeSystem(
        ComponentRegistry& registry,
        std::string_view name,
        const std::span<ComponentHandle>& filterComponents,
        const std::span<ComponentHandle>& resultComponents);

    SequenceSystem& ModifyFilterSystem() { return _filterSystem; }
    const SequenceSystem& GetFilterSystem() const { return _filterSystem; }

    void SetFilterFunction(std::function<bool(const SystemContext&)> function) { _filterFunction = std::move(function); }
    void SetResultFunction(std::function<void(const SystemContext&)> function) { _resultFunction = std::move(function); }

    void Process() override;

private:
    std::string _filterSystemName;
    SequenceSystem _filterSystem;
    std::vector<CellId> _filteredIds;
    std::function<bool(const SystemContext&)> _filterFunction;
    std::function<void(const SystemContext&)> _resultFunction;
};