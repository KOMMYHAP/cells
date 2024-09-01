#pragma once
#include "base_widget.h"
#include "menu_widgets/base_menu_widget.h"
#include "menu_widgets/menu_widget_id.h"

class MenuRootWidget final : public BaseWidget {
public:
    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    MenuWidgetId AddWidget(std::string name, Args&&... args);

    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    MenuWidgetId AddWidget(MenuWidgetId parent, std::string name, Args&&... args);

    void UpdateWidget(sf::Time elapsedTime) override;

private:
    static constexpr auto RootWidgetId = static_cast<MenuWidgetId>(std::numeric_limits<std::underlying_type_t<MenuWidgetId>>::max());

    struct WidgetState {
        bool opened { false };
        bool justOpened { false };
        bool justOpenedFirstTime { false };
        bool wasOpenedAtLeastOnce { false };
    };
    struct WidgetData {
        WidgetState state;
        std::unique_ptr<BaseMenuWidget> widget;
        std::string name;
    };
    struct WidgetsGroup {
        std::vector<MenuWidgetId> items;
    };

    MenuWidgetId AddWidget(MenuWidgetId parent, WidgetData widgetData);

    WidgetData& ModifyWidgetData(MenuWidgetId id);
    const WidgetData& GetWidgetData(MenuWidgetId id) const;
    const WidgetsGroup* FindWidgetGroup(MenuWidgetId id) const;

    bool UpdateWidgetsGroup(MenuWidgetId id);
    bool ProcessWidgetState(MenuWidgetId id);

    void UpdateOpenedWidgets(sf::Time elapsedTime);
    bool ProcessOpenedWidgetState(MenuWidgetId id, sf::Time elapsedTime);

    std::vector<WidgetData> _widgets;
    std::map<MenuWidgetId, WidgetsGroup> _indexedGroups;
    MenuWidgetId _rootWidgetId { RootWidgetId };
    std::vector<MenuWidgetId> _openedWidgets;
};

#include "menu_root_widget.hpp"