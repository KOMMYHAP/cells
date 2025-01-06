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

LuaMenuWidget* LuaMenu::Register(MenuWidgetId parentId, std::string_view name)
{
    if (!_menuRootWidget->HasWidget(parentId)) {
        _logger->Error("Unknown parent widget!");
        return nullptr;
    }
    auto&& [id, widget] = _menuRootWidget->AddWidget<LuaMenuWidget>(parentId, std::string(name), *_logger);
    if (id == MenuWidgetId::Invalid) {
        _logger->Error("Failed to register new menu widget!");
        return nullptr;
    }
    widget->SetName(name);
    widget->SetId(id);
    _widgets.emplace_back(widget);
    return widget;
}

void LuaMenu::OpenWidget(const LuaMenuWidget* widget)
{
    _menuRootWidget->OpenWidget(widget->GetId());
}

void LuaMenu::CloseWidget(const LuaMenuWidget* widget)
{
    _menuRootWidget->CloseWidget(widget->GetId());
}