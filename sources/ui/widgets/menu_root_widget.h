﻿#pragma once
#include "base_widget.h"
#include "menu_widgets/base_menu_widget.h"
#include "menu_widgets/menu_widget_id.h"

class MenuRootWidget final : public BaseWidget {
public:
    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    std::pair<MenuWidgetId, T*> AddWidget(std::string name, Args&&... args);

    template <class T, class... Args>
        requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
    std::pair<MenuWidgetId, T*> AddWidget(MenuWidgetId parent, std::string name, Args&&... args);

    bool HasWidget(MenuWidgetId id) const;

    void OpenWidget(MenuWidgetId id);
    void CloseWidget(MenuWidgetId id);

    void UpdateWidget(Common::Time elapsedTime) override;

private:
    static constexpr size_t LimitNumberOfWidgets = 1'000;
    struct WidgetState {
        bool opened { false };
        bool justOpenedFirstTime { false };
        bool justOpened { false };
        bool justClosed { false };
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

    void UpdateWidgetsGroup(MenuWidgetId id);
    void ProcessWidgetState(MenuWidgetId id);
    void ChangeWidgetState(MenuWidgetId id, bool openedNow, bool wasOpen);

    void UpdateOpenedWidgets(Common::Time elapsedTime);
    bool ProcessOpenedWidgetState(MenuWidgetId id, Common::Time elapsedTime);

    std::vector<WidgetData> _widgets;
    std::map<MenuWidgetId, WidgetsGroup> _indexedGroups;
    std::vector<MenuWidgetId> _openedWidgets;
};

#include "menu_root_widget.hpp"