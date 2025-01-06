#include "lua_system.h"

#include "bind/lua_imgui_bindings.h"
#include "bind/lua_menu.h"
#include "bind/lua_menu_widget.h"
#include "implot.h"

namespace {
bool ShowImGuiDemoWindow()
{
    bool opened = true;
    ImGui::ShowDemoWindow(&opened);
    return opened;
}

bool ShowImPlotDemoWindow()
{
    bool opened = true;
    ImPlot::ShowDemoWindow(&opened);
    return opened;
}
}

LuaSystem::LuaSystem(LuaLogger& logger)
    : _logger(&logger)
{
    _luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);
}

void LuaSystem::RegisterWidgets(MenuRootWidget& menuRootWidget)
{
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
    _luaState["ui_menu"] = LuaMenu(*_logger, menuRootWidget);

    sol_ImGui::Init(_luaState);

    sol::table ImPlot = _luaState.create_named_table("ImPlot");
    ImPlot.set_function("ShowDemo", &ShowImPlotDemoWindow);

    sol::table ImGui = _luaState["ImGui"];
    ImGui.set_function("ShowDemo", &ShowImGuiDemoWindow);
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