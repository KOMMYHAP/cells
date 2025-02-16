#pragma once
#include "root_widget.h"

template <class WidgetType, class... Args>
    requires std::is_constructible_v<WidgetType, Args...>
WidgetType& RootWidget::EmplaceWidget(Args&&... args)
{
    auto widget = std::make_unique<WidgetType>(std::forward<Args>(args)...);
    return AddWidget(std::move(widget));
}

template <class WidgetType>
    requires(!std::same_as<WidgetType, BaseWidget>)
WidgetType& RootWidget::AddWidget(std::unique_ptr<WidgetType> widget)
{
    _widgets.emplace_back(std::move(widget));
    WidgetType& widgetRef = *widget;
    if constexpr (std::is_base_of_v<CustomRenderWidget, WidgetType>) {
        _customRenderWidgets.emplace_back(&widgetRef);
    }
    return widgetRef;
}