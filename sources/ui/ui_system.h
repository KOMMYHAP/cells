#pragma once

#include "drawable.h"
#include "ui_layout.h"

class UiSystem : public Drawable {
public:
    UiSystem(sf::RenderTarget& renderTarget, const UiLayout& uiLayout, const common::Storage& systems);

    void Draw() override;

private:
    sf::RenderTarget& _renderTarget;
    std::unique_ptr<WorldWidget> _worldWidget;
    const UiLayout _uiLayout;
};
