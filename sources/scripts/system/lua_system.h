#pragma once
#include "lua_logger.h"
#include "sol/forward.hpp"

class MenuRootWidget;

class LuaSystem {
public:
    explicit LuaSystem(LuaLogger& logger);

    void RegisterWidgets(MenuRootWidget& menuRootWidget);

    void SetPath(const std::filesystem::path& path);
    sol::load_result* LoadScript(std::string_view scriptName);
    sol::load_result* FindScript(std::string_view scriptName);

    template <class... Args>
    sol::function_result RunScript(std::string_view scriptName, Args&&... args);
    sol::function_result RunScriptFromCode(std::string_view script);

private:
    sol::state _luaState;
    gsl::not_null<LuaLogger*> _logger;
    std::map<std::string, sol::load_result, std::less<>> _scripts;
    std::filesystem::path _scriptsLoaderPath;
};

#include "lua_system.hpp"