#pragma once
#include "base_widget.h"
#include "custom_render_widget.h"

class RootWidget final : public CustomRenderWidget {
public:
    ~RootWidget() override;

    template <class WidgetType, class... Args>
        requires std::is_constructible_v<WidgetType, Args...>
    WidgetType& AddWidget(Args&&... args);

    void UpdateWidget(Common::Time elapsedTime) override;
    void RenderWidget() override;

private:
    std::vector<std::unique_ptr<BaseWidget>> _widgets;
    std::vector<CustomRenderWidget*> _customRenderWidgets;
};

#include "root_widget.hpp"