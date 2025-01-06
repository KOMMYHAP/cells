#pragma once
#include "menu_widgets/menu_widget_id.h"

class LuaMenuWidget;
class MenuRootWidget;
class LuaLogger;
class LuaMenu {
public:
    LuaMenu(LuaLogger& logger, MenuRootWidget& menuRootWidget);
    ~LuaMenu();

    LuaMenuWidget* Register(MenuWidgetId parentId, std::string_view name);
    void OpenWidget(const LuaMenuWidget* widget);
    void CloseWidget(const LuaMenuWidget* widget);

private:
    gsl::not_null<LuaLogger*> _logger;
    gsl::not_null<MenuRootWidget*> _menuRootWidget;
    std::vector<LuaMenuWidget*> _widgets;
};
