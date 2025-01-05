#include "lua_registrable_system.h"

#include "system/lua_system.h"
#include "system/ui_system.h"

LuaRegistrableSystem::LuaRegistrableSystem() = default;
LuaRegistrableSystem::~LuaRegistrableSystem() = default;

std::error_code LuaRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    UiSystem& uiSystem = storage.Modify<UiSystem>();
    MenuRootWidget& menuRootWidget = uiSystem.GetMenuRootWidget();
    _luaSystem = std::make_unique<LuaSystem>(menuRootWidget);
    return {};
}

void LuaRegistrableSystem::TerminateSystem()
{
    _luaSystem.reset();
}