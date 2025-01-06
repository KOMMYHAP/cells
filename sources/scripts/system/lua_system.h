#pragma once
#include "lua_logger.h"
#include "sol/forward.hpp"

class MenuRootWidget;

class LuaSystem {
public:
    explicit LuaSystem(LuaLogger& logger);

    void RegisterWidgets(MenuRootWidget& menuRootWidget);

    void AddPath(const std::filesystem::path& path);
    sol::load_result* LoadScript(const std::filesystem::path& scriptPath);
    sol::load_result* FindScript(const std::string& scriptName);

    sol::function_result RunScript(std::string_view script);

private:
    sol::state _luaState;
    gsl::not_null<LuaLogger*> _logger;
    std::map<std::string, sol::load_result> _scripts;
};