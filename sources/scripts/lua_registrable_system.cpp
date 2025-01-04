#include "lua_registrable_system.h"

#include "lua_system.h"

LuaRegistrableSystem::LuaRegistrableSystem() = default;
LuaRegistrableSystem::~LuaRegistrableSystem() = default;

std::error_code LuaRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    _luaSystem = std::make_unique<LuaSystem>();
    return {};
}
void LuaRegistrableSystem::TerminateSystem()
{
    _luaSystem.reset();
}