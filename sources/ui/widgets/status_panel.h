#pragma once

#include "sample_counter.h"
#include "ui_layout.h"
#include "custom_render_widget.h"

class World;

class StatusPanel final : public CustomRenderWidget {
public:
    StatusPanel(const UiLayout& layout, const sf::Font& font, World& world);

    void UpdateWidget(sf::Time elapsedTime) override;
    void RenderWidget(sf::RenderTarget& target) override;

private:
    sf::Text _text;
    std::string _buffer;
    common::SampleCounter<float, 10> _frameTimeCounter;
    UiLayout _layout;
    World& _world;
};