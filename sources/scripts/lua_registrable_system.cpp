#include "lua_registrable_system.h"

#include "system/lua_logger.h"
#include "system/lua_system.h"
#include "system/ui_system.h"

LuaRegistrableSystem::LuaRegistrableSystem() = default;
LuaRegistrableSystem::~LuaRegistrableSystem() = default;

std::error_code LuaRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    UiSystem& uiSystem = storage.Modify<UiSystem>();
    MenuRootWidget& menuRootWidget = uiSystem.ModifyMenuRootWidget();

    MakeDefaultLuaLogger();
    LuaLogger* logger { _defaultLogger.get() };
    if (storage.Has<LuaLogger>()) {
        logger = &storage.Modify<LuaLogger>();
    }

    _luaSystem = std::make_unique<LuaSystem>(*logger);
    _luaSystem->RegisterWidgets(menuRootWidget);
    return {};
}

void LuaRegistrableSystem::TerminateSystem()
{
    _luaSystem.reset();
}

void LuaRegistrableSystem::MakeDefaultLuaLogger()
{
    _defaultLogger = std::make_unique<LuaLogger>([] {
        std::fflush(stdout);
        ASSERT_FAIL("Lua error occurred!");
    });
}