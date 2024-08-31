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

void RootWidget::Update(sf::Time elapsedTime)
{
    ImGui::SFML::Update(*_window, elapsedTime);
    for (const auto& widget : _widgets) {
        widget->Update(elapsedTime);
    }
}

void RootWidget::Draw(sf::RenderTarget& target)
{
    for (const auto& widget : _widgets) {
        widget->Draw(target);
    }
    ImGui::SFML::Render(*_window);
}