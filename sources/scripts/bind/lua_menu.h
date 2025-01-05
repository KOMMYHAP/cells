#pragma once
#include "menu_widgets/menu_widget_id.h"

class LuaMenuWidget;
class MenuRootWidget;
class LuaLogger;
class LuaMenu {
public:
    LuaMenu(LuaLogger& logger, MenuRootWidget& menuRootWidget);
    ~LuaMenu();

    std::pair<std::underlying_type_t<MenuWidgetId>, LuaMenuWidget*> Register(sol::stack_object luaParentWidget, std::string_view name);

    uint32_t RootWidget {std::to_underlying(MenuWidgetId::Root)};
    uint32_t InvalidWidget {std::to_underlying(MenuWidgetId::Invalid)};

private:
    gsl::not_null<LuaLogger*> _logger;
    gsl::not_null<MenuRootWidget*> _menuRootWidget;
    std::vector<LuaMenuWidget*> _widgets;
};
