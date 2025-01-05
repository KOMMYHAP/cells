#include "lua_menu_widget.h"

#include "system/lua_logger.h"

LuaMenuWidget::LuaMenuWidget(LuaLogger& logger)
    : _logger(&logger)
{
}

void LuaMenuWidget::OnMenuItemOpenedFirstTime()
{
    if (_onFirstTimeOpen == sol::nil) {
        return;
    }
    if (const sol::function_result result = _onFirstTimeOpen(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}: {}!", KeyOnFirstTimeOpen, err.what());
    }
}

void LuaMenuWidget::OnMenuItemJustOpened()
{
    if (_onJustOpened == sol::nil) {
        return;
    }
    if (const sol::function_result result = _onJustOpened(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}: {}!", KeyOnJustOpen, err.what());
    }
}

BaseMenuWidget::MenuWidgetAction LuaMenuWidget::ProcessMenuItem(Common::Time elapsedTime)
{
    if (_onUpdate == sol::nil) {
        return MenuWidgetAction::KeepOpen;
    }
    const sol::function_result result = _onUpdate();
    if (!result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}: {}!", KeyOnUpdate, err.what());
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
    if (_onClosed == sol::nil) {
        return;
    }
    if (const sol::function_result result = _onClosed(); !result.valid()) {
        const sol::error err = result;
        _logger->Error("Failed to call {}: {}!", KeyOnClosed, err.what());
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

void LuaMenuWidget::ClearFunctions()
{
    _onFirstTimeOpen = sol::nil;
    _onJustOpened = sol::nil;
    _onUpdate = sol::nil;
    _onClosed = sol::nil;
}