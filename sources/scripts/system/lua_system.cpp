#include "lua_system.h"

#include "bind/lua_imgui_bindings.h"
#include "bind/lua_menu.h"
#include "bind/lua_menu_widget.h"

LuaSystem::LuaSystem(MenuRootWidget& menuRootWidget, LuaLogger& logger)
    : _logger(&logger)
    , _menuRootWidget(&menuRootWidget)
{
    _luaState.open_libraries(sol::lib::base);

    {
        _luaState.new_usertype<LuaMenuWidget>("menu_widget",
            sol::meta_function::new_index,
            &LuaMenuWidget::OverrideFunction);
    }
    {
        sol::usertype<LuaMenu> luaMenuTable = _luaState.new_usertype<LuaMenu>("menu",
            "register", &LuaMenu::Register,
            "root_widget", sol::readonly(&LuaMenu::RootWidget),
            "invalid_widget", sol::readonly(&LuaMenu::InvalidWidget));
    }

    _luaState["ui_menu"] = LuaMenu(*_logger, *_menuRootWidget);

    sol_ImGui::Init(_luaState);
}

sol::function_result LuaSystem::RunScript(std::string_view script)
{
    sol::function_result result = _luaState.script(script);
    if (!result.valid()) {
        const sol::error error = result;
        _logger->Error("Failed to execute script!\n{}", error.what());
        default_traceback_error_handler(_luaState);
    }
    return result;
}