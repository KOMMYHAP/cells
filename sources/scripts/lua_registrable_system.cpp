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
    _luaSystem = &storage.Store<LuaSystem>(*logger);
    _luaSystem->RegisterWidgets(menuRootWidget);

    const Config& config = storage.Get<Config>();
    _luaSystem->SetPath(config.luaDirectory);

    if (config.startupScript) {
        const std::string_view scriptName = *config.startupScript;
        _luaSystem->LoadScript(scriptName);
        if (const sol::function_result r = _luaSystem->RunScript(scriptName); !r.valid()) {
            return std::make_error_code(std::errc::invalid_argument);
        }
    }
    return {};
}

void LuaRegistrableSystem::TerminateSystem()
{
    _luaSystem = nullptr;
}

void LuaRegistrableSystem::MakeDefaultLuaLogger()
{
    _defaultLogger = std::make_unique<LuaLogger>([] {
        std::fflush(stdout);
        ASSERT_FAIL("Lua error occurred!");
    });
}