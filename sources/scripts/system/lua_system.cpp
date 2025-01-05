#include "lua_system.h"

#include "bind/lua_menu_widget.h"
#include "menu_widgets/menu_widget_id.h"
#include "widgets/menu_root_widget.h"

LuaSystem::LuaSystem(MenuRootWidget& menuRootWidget)
    : _menuRootWidget(&menuRootWidget)
{
    _luaState.new_usertype<LuaMenuWidget>("menu_widget",
        sol::meta_function::new_index,
        &LuaMenuWidget::OverrideFunction);

    _luaState["ui_menu"].set_function("register", [this](sol::object luaParentWidget, std::string_view name) {
        auto parentWidgetId { MenuWidgetId::Root };
        if (luaParentWidget.is<uint32_t>) {
            parentWidgetId = static_cast<MenuWidgetId>(luaParentWidget.as<uint32_t>());
        } else if (luaParentWidget != sol::lua_nil) {
            _logger.Error("Invalid id of menu widget");
            return std::to_underlying(MenuWidgetId::Invalid);
        }
        const MenuWidgetId id = _menuRootWidget->AddWidget<LuaMenuWidget>(parentWidgetId, std::string(name), _logger);
        if (id == MenuWidgetId::Invalid) {
            _logger.Error("Failed to register new menu widget!");
        }
        return std::to_underlying(id);
    });
}