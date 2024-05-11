#pragma once

#include "sample_counter.h"
#include "ui_layout.h"

class World;

class StatusPanel {
public:
    StatusPanel(const UiLayout & layout, const sf::Font & font, World & world);

    void Update(sf::Time elapsedTime);
    void Draw(sf::RenderTarget& target);

private:
    sf::Text _text;
    std::string _buffer;
    common::SampleCounter<float, 10> _frameTimeCounter;
    UiLayout _layout;
    World& _world;
};