#include "lua_menu.h"

#include "lua_menu_widget.h"
#include "system/lua_logger.h"
#include "widgets/menu_root_widget.h"

LuaMenu::LuaMenu(LuaLogger& logger, MenuRootWidget& menuRootWidget)
    : _logger(&logger)
    , _menuRootWidget(&menuRootWidget)
{
}

LuaMenu::~LuaMenu()
{
    for (LuaMenuWidget* widget : _widgets) {
        widget->ClearFunctions();
    }
}

std::pair<std::underlying_type_t<MenuWidgetId>, LuaMenuWidget*> LuaMenu::Register(sol::stack_object luaParentWidget, std::string_view name)
{
    auto parentWidgetId { MenuWidgetId::Root };
    if (luaParentWidget.is<uint32_t>()) {
        parentWidgetId = static_cast<MenuWidgetId>(luaParentWidget.as<uint32_t>());
    } else if (luaParentWidget != sol::lua_nil) {
        _logger->Error("Invalid id of menu widget!");
        return std::make_pair(std::to_underlying(MenuWidgetId::Invalid), nullptr);
    }
    if (!_menuRootWidget->HasWidget(parentWidgetId)) {
        _logger->Error("Unknown parent widget!");
        return std::make_pair(std::to_underlying(MenuWidgetId::Invalid), nullptr);
    }
    auto&& [id, widget] = _menuRootWidget->AddWidget<LuaMenuWidget>(parentWidgetId, std::string(name), *_logger);
    if (id == MenuWidgetId::Invalid) {
        _logger->Error("Failed to register new menu widget!");
    }
    _widgets.emplace_back(widget);
    return std::make_pair(std::to_underlying(id), widget);
}