#pragma once
#include "base_menu_widget.h"

class ImGuiDemoMenuWidget final : public BaseMenuWidget {
public:
    MenuWidgetAction ProcessMenuItem(sf::Time elapsedTime) override;
};