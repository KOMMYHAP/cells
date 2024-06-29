#pragma once

#include "sample_counter.h"
#include "ui_layout.h"
#include "ui_widget.h"

class World;

class StatusPanel : public UiWidget {
public:
    StatusPanel(const UiLayout& layout, const sf::Font& font, World& world);

    void Update(sf::Time elapsedTime) override;
    void Draw(sf::RenderTarget& target) override;

private:
    sf::Text _text;
    std::string _buffer;
    common::SampleCounter<float, 10> _frameTimeCounter;
    UiLayout _layout;
    World& _world;
};