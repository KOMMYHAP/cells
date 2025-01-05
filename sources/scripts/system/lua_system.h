#pragma once
#include "lua_logger.h"

class MenuRootWidget;

class LuaSystem {
public:
    LuaSystem(MenuRootWidget& menuRootWidget);

private:
    sol::state _luaState;
    LuaLogger _logger;
    gsl::not_null<MenuRootWidget*> _menuRootWidget;
};