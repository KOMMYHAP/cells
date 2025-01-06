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

    static constexpr auto RelativePathToLuaDirectory = "../../../../sources/scripts/lua/"sv;
    const std::filesystem::path scriptsDirectory = std::filesystem::current_path() / RelativePathToLuaDirectory;
    _luaSystem->AddPath(scriptsDirectory);

    sol::load_result* loader = _luaSystem->LoadScript(scriptsDirectory / "loader.lua"sv);
    if (!loader) {
        return std::make_error_code(std::errc::invalid_argument);
    }
    if (!loader->valid()) {
        return std::make_error_code(std::errc::invalid_argument);
    }
    const sol::function_result r = loader->call();
    if (!r.valid()) {
        logger->Error("Failed to execute script: {}!", sol::error { r }.what());
        return std::make_error_code(std::errc::invalid_argument);
    }
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