#pragma once
#include "base_widget.h"

class CustomRenderWidget : public BaseWidget {
public:
    virtual void RenderWidget(sf::RenderTarget& target) = 0;
};