#pragma once
#include "ui_widget.h"

class RootWidget final : public UiWidget {
public:
    explicit RootWidget(sf::RenderWindow& window);
    ~RootWidget() override;

    template <class WidgetType, class... Args>
        requires std::is_constructible_v<WidgetType, Args...>
    WidgetType& AddWidget(Args&&... args);

    void Update(sf::Time elapsedTime) override;
    void Draw(sf::RenderTarget& target) override;

private:
    gsl::not_null<sf::RenderWindow*> _window;
    std::vector<std::unique_ptr<UiWidget>> _widgets;
};

template <class WidgetType, class... Args>
    requires std::is_constructible_v<WidgetType, Args...>
WidgetType& RootWidget::AddWidget(Args&&... args)
{
    auto widget = std::make_unique<WidgetType>(std::forward<Args>(args)...);
    WidgetType& widgetRef = *widget;
    _widgets.emplace_back(std::move(widget));
    return widgetRef;
}