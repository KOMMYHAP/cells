#include "lua_registrable_system.h"

#include <sol/sol.hpp>

#include "storage/stack_storage.h"

#include "lua_system_impl.h"

namespace scripts {

LuaSystem::LuaSystem() = default;
LuaSystem::~LuaSystem() = default;

std::error_code LuaSystem::InitializeSystem(common::StackStorage& storage)
{
    _impl = std::make_unique<LuaSystemImpl>(storage);
    return {};
}

void LuaSystem::TerminateSystem()
{
    _impl.reset();
}

}