#pragma once
#include "root_widget.h"

template <class WidgetType, class... Args>
    requires std::is_constructible_v<WidgetType, Args...>
WidgetType& RootWidget::AddWidget(Args&&... args)
{
    auto widget = std::make_unique<WidgetType>(std::forward<Args>(args)...);
    WidgetType& widgetRef = *widget;
    _widgets.emplace_back(std::move(widget));
    if constexpr (std::is_base_of_v<CustomRenderWidget, WidgetType>) {
        _customRenderWidgets.emplace_back(&widgetRef);
    }
    return widgetRef;
}