#pragma once

#include <sol/forward.hpp>

#include "systems/system_registry.h"

namespace scripts {



class LuaSystemFactory {
public:
    LuaSystemFactory(SystemRegistry& systemRegistry, ComponentRegistry& componentRegistry);

    SystemHandle MakeSystem(sol::this_state state, std::string_view name, sol::protected_function function, const sol::variadic_args& args);

private:
    struct LuaSystemStorage {
        LuaSystemFactory& systemFactory;
        std::vector<Component> components;
        sol::this_state state;
        sol::protected_function function;
    };
    void ProcessSystem(LuaSystemStorage & storage, const SystemContext & context);

    SystemRegistry& _registry;
    ComponentRegistry& _componentRegistry;
};

}