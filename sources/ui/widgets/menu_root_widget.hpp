#pragma once
#include "menu_root_widget.h"

template <class T, class... Args>
    requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
MenuWidgetId MenuRootWidget::AddWidget(std::string name, Args&&... args)
{
    return LinkWidget<T>(RootWidgetId, std::move(name), std::forward<Args>(args)...);
}

template <class T, class... Args>
    requires std::is_base_of_v<BaseMenuWidget, T> && std::is_constructible_v<T, Args...>
MenuWidgetId MenuRootWidget::LinkWidget(MenuWidgetId parent, std::string name, Args&&... args)
{
    WidgetData data;
    data.widget = std::make_unique<T>(std::forward<Args>(args)...);
    data.name = std::move(name);
    return AddWidget(parent, std::move(data));
}
