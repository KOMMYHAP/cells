#include "lua_menu_widget.h"

#include "system/lua_logger.h"

LuaMenuWidget::LuaMenuWidget(LuaLogger& logger)
    : _logger(&logger)
{
}

void LuaMenuWidget::OnMenuItemOpenedFirstTime()
{
    if (const sol::function_result result = _onFirstTimeOpen(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}!", KeyOnFirstTimeOpen);
    }
}

void LuaMenuWidget::OnMenuItemJustOpened()
{
    if (const sol::function_result result = _onJustOpened(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}!", KeyOnJustOpen);
    }
}

BaseMenuWidget::MenuWidgetAction LuaMenuWidget::ProcessMenuItem(Common::Time elapsedTime)
{
    const sol::function_result result = _onUpdate();
    if (!result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}!", KeyOnUpdate);
        return MenuWidgetAction::KeepOpen;
    }

    if (result.get_type() != sol::type::boolean) {
        _logger->Error("Wrong return type of {}!", KeyOnUpdate);
        return MenuWidgetAction::KeepOpen;
    }

    const bool keepOpen = result;
    return keepOpen ? MenuWidgetAction::KeepOpen : MenuWidgetAction::ShouldClose;
}

void LuaMenuWidget::OnMenuItemJustClosed()
{
    if (const sol::function_result result = _onClosed(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}!", KeyOnClosed);
    }
}

void LuaMenuWidget::OverrideFunction(sol::string_view key, sol::function function, sol::this_state /*state*/)
{
    sol::function* targetFunction = nullptr;
    if (key == KeyOnFirstTimeOpen) {
        targetFunction = &_onFirstTimeOpen;
    } else if (key == KeyOnJustOpen) {
        targetFunction = &_onJustOpened;
    } else if (key == KeyOnUpdate) {
        targetFunction = &_onUpdate;
    } else if (key == KeyOnClosed) {
        targetFunction = &_onClosed;
    }

    if (!targetFunction) {
        _logger->Error("Unsupported key \"{}\" of lua menu widget!", key);
        return;
    }

    *targetFunction = std::move(function);
}