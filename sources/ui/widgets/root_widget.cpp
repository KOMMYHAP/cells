#include "root_widget.h"

RootWidget::~RootWidget()
{
    for (auto& widget : std::ranges::reverse_view(_widgets)) {
        widget.reset();
    }
}

void RootWidget::UpdateWidget(sf::Time elapsedTime)
{
    for (const auto& widget : _widgets) {
        widget->UpdateWidget(elapsedTime);
    }
}

void RootWidget::RenderWidget()
{
    for (CustomRenderWidget* widget : _customRenderWidgets) {
        widget->RenderWidget();
    }
}