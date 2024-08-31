#include "root_widget.h"

RootWidget::RootWidget(sf::RenderWindow& window)
    : _window(&window)
{
}
RootWidget::~RootWidget()
{
    for (auto& widget : std::ranges::reverse_view(_widgets)) {
        widget.reset();
    }
}

void RootWidget::UpdateWidget(sf::Time elapsedTime)
{
    ImGui::SFML::Update(*_window, elapsedTime);
    for (const auto& widget : _widgets) {
        widget->UpdateWidget(elapsedTime);
    }
}

void RootWidget::RenderWidget(sf::RenderTarget& target)
{
    for (CustomRenderWidget* widget : _customRenderWidgets) {
        widget->RenderWidget(target);
    }
    ImGui::SFML::Render(*_window);
}