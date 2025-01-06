#include "lua_system.h"

#include "bind/lua_imgui_bindings.h"
#include "bind/lua_menu.h"
#include "bind/lua_menu_widget.h"

LuaSystem::LuaSystem(MenuRootWidget& menuRootWidget, LuaLogger& logger)
    : _logger(&logger)
    , _menuRootWidget(&menuRootWidget)
{
    _luaState.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);
    _luaState.new_usertype<LuaMenuWidget>("menu_widget",
        sol::meta_function::new_index,
        &LuaMenuWidget::OverrideFunction,
        "name", sol::readonly_property(&LuaMenuWidget::GetName),
        "id", sol::readonly_property(&LuaMenuWidget::GetId));
    _luaState.new_usertype<MenuWidgetId>("widget_id");
    _luaState.new_usertype<LuaMenu>("menu",
        "register", &LuaMenu::Register,
        "open", &LuaMenu::OpenWidget,
        "close", &LuaMenu::CloseWidget,
        "root_widget", sol::var(std::to_underlying(MenuWidgetId::Root)),
        "invalid_widget", sol::var(std::to_underlying(MenuWidgetId::Invalid)));
    _luaState["ui_menu"] = LuaMenu(*_logger, *_menuRootWidget);

    sol_ImGui::Init(_luaState);
}

sol::function_result LuaSystem::RunScript(std::string_view script)
{
    sol::function_result result = _luaState.script(script, sol::script_pass_on_error);
    if (!result.valid()) {
        const sol::error error = result;
        _logger->Error("Failed to execute script!\n{}", error.what());
    }
    return result;
}