#pragma once

#include "components/component_handle.h"
#include "components/component_registry.h"

class ComponentStorage;
class ComponentRegistry;

class System {
public:
    System(ComponentRegistry& registry, const std::vector<ComponentHandle>& handles);

    struct Context {
        const std::vector<std::byte*>& components;
    };
    void Foreach(const std::function<void(const Context&)>& func);

private:
    struct ComponentInfo {
        ComponentHandle handle;
        const Component* meta { nullptr };
        std::byte* startAddress { nullptr };
    };

    ComponentRegistry& _componentRegistry;
    std::vector<ComponentInfo> _componentInfoList;
    std::vector<std::byte*> _componentBuffer;
};