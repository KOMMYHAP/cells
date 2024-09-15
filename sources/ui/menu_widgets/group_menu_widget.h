#pragma once
#include "base_menu_widget.h"

class GroupMenuWidget final : public BaseMenuWidget {
public:
    MenuWidgetAction ProcessMenuItem(Common::Time elapsedTime) override { return MenuWidgetAction::KeepOpen; }
};