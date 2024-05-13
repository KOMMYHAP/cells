#pragma once

#include "components/component_handle.h"
#include "components/component_registry.h"

class ComponentStorage;
class ComponentRegistry;

class System {
public:
    System() = default;
    System(const ComponentRegistry& registry, const std::span<ComponentHandle>& handles);

    struct Context {
        CellId id;
        const std::vector<std::byte*>& components;
    };
    void Foreach(const std::function<void(const Context&)>& func);

    void Message(const std::span<CellId>& cells, const std::function<void(const Context&)>& func);

private:
    struct ComponentInfo {
        ComponentHandle handle;
        uint16_t sizeInBytes { 0 };
        std::byte* startAddress { nullptr };
    };

    uint32_t _cellsCount { 0 };
    std::vector<ComponentInfo> _componentInfoList;
    std::vector<std::byte*> _componentBuffer;
};