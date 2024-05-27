#pragma once

#include <sol/sol.hpp>

#include "lua_component_factory.h"
#include "lua_system_factory.h"

namespace scripts {

class LuaSystemImpl {
public:
    LuaSystemImpl(common::StackStorage& storage);

    sol::state& ModifyLuaState() { return luaState; }
    const sol::state& GetLuaState() const { return luaState; }

private:
    sol::state luaState;
    LuaSystemFactory systemFactory;
    LuaComponentFactory componentFactory;
};

}