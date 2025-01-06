#pragma once
#include "lua_logger.h"
#include "sol/forward.hpp"

class MenuRootWidget;

class LuaSystem {
public:
    explicit LuaSystem(LuaLogger& logger);

    void RegisterWidgets(MenuRootWidget& menuRootWidget);

    sol::function_result RunScript(std::string_view script);

private:
    sol::state _luaState;
    gsl::not_null<LuaLogger*> _logger;
};