#pragma once

#include "systems/system_context.h"

#include "components/cell_id.h"
#include "components/component_handle.h"
#include "components/component_registry.h"

class SystemBase {
public:
    virtual ~SystemBase() = default;
    SystemBase(ComponentRegistry& registry, std::string_view name, const std::span<ComponentHandle>& handles);

    std::string_view GetName() const { return _name; }

    virtual void Process() = 0;

protected:
    void ProcessImpl(std::span<const CellId> cells, const std::function<void(const SystemContext&)>& func);

private:
    struct ComponentInfo {
        ComponentHandle handle;
        uint16_t sizeInBytes { 0 };
        std::byte* startAddress { nullptr };
    };

    std::string_view _name;
    uint32_t _cellsCount { 0 };
    std::vector<ComponentInfo> _componentInfoList;
    std::vector<std::byte*> _componentBuffer;
};