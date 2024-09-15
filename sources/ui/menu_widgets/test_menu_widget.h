#pragma once
#include "base_menu_widget.h"

class TestMenuWidget final : public BaseMenuWidget {
public:
    void OnMenuItemOpenedFirstTime() override;
    void OnMenuItemJustOpened() override;
    MenuWidgetAction ProcessMenuItem(Common::Time elapsedTime) override;
    void OnMenuItemJustClosed() override;

private:
    void Flush();
    
    Common::Time _elapsedTimeSinceFlush;
};