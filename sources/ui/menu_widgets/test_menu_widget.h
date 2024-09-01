#pragma once
#include "base_menu_widget.h"

class TestMenuWidget final : public BaseMenuWidget {
public:
    void OnMenuItemOpenedFirstTime() override;
    void OnMenuItemJustOpened() override;
    MenuWidgetAction ProcessMenuItem(sf::Time elapsedTime) override;
    void OnMenuItemJustClosed() override;

private:
    void Flush();
    
    sf::Time _elapsedTimeSinceFlush;
};