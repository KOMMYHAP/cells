#pragma once
#include "time/time.h"

class BaseMenuWidget {
public:
    virtual ~BaseMenuWidget() = default;

    virtual void OnMenuItemOpenedFirstTime() { }
    virtual void OnMenuItemJustOpened() { }

    enum class MenuWidgetAction {
        KeepOpen,
        ShouldClose,
    };
    virtual MenuWidgetAction ProcessMenuItem(Common::Time /*elapsedTime*/) { return MenuWidgetAction::ShouldClose; }

    virtual void OnMenuItemJustClosed() { }
};