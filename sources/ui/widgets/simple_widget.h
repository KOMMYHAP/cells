#pragma once
#include "ui_widget.h"

class SimpleWidget : public UiWidget {
public:
    void Draw(sf::RenderTarget& /*target*/) final {}
};