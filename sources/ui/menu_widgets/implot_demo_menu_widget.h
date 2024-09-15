#pragma once
#include "base_menu_widget.h"

class ImPlotDemoMenuWidget final : public BaseMenuWidget {
public:
    MenuWidgetAction ProcessMenuItem(Common::Time elapsedTime) override;
};