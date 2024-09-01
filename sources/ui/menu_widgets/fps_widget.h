#pragma once
#include "base_menu_widget.h"
#include "sample_counter.h"

class FpsWidget final : public BaseMenuWidget {
public:
    MenuWidgetAction ProcessMenuItem(sf::Time elapsedTime) override;

private:
    common::SampleCounter<int32_t, 30> _frameTimeCounter;
};