#pragma once

#include "components/component_registry.h"

namespace scripts {

class LuaComponentFactory {
public:
    LuaComponentFactory(ComponentRegistry& componentRegistry);
    ComponentHandle MakeComponent(std::string_view name, uint8_t numbersCount);

private:
    ComponentRegistry& _registry;
};

}