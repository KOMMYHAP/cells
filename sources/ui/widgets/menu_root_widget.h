#pragma once
#include "base_widget.h"
#include "menu_widgets/menu_widget_id.h"
#include "menu_widgets/base_menu_widget.h"

class MenuRootWidget final : public BaseWidget {
public:
    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    MenuWidgetId AddWidget(std::string name, Args&&... args);

    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    MenuWidgetId LinkWidget(MenuWidgetId parent, std::string name, Args&&... args);

    void UpdateWidget(sf::Time elapsedTime) override;

private:
    static constexpr auto RootWidgetId = static_cast<MenuWidgetId>(std::numeric_limits<std::underlying_type_t<MenuWidgetId>>::max());

    struct WidgetState {
        bool wasOpenedAtLeastOnce { false };
        bool wasOpened { false };
    };
    struct WidgetData {
        WidgetState state;
        std::unique_ptr<BaseMenuWidget> widget;
        std::string name;
    };
    struct WidgetsGroup {
        std::vector<MenuWidgetId> items;
    };
    using WidgetNextGroup = std::optional<MenuWidgetId>;

    MenuWidgetId AddWidget(MenuWidgetId parent, WidgetData widgetData);

    void UpdateWidgetsGroup(MenuWidgetId id, sf::Time elapsedTime);
    WidgetNextGroup UpdateWidget(MenuWidgetId id, sf::Time elapsedTime);

    void ProcessWidgetAction(WidgetData& widgetData, sf::Time elapsedTime);

    std::vector<WidgetData> _widgets;
    std::map<MenuWidgetId, WidgetsGroup> _indexedGroups;
    MenuWidgetId _rootEntry { RootWidgetId };
};

#include "menu_root_widget.hpp"