#pragma once
#include "menu_widgets/base_menu_widget.h"

class LuaLogger;

class LuaMenuWidget final : public BaseMenuWidget {
public:
    explicit LuaMenuWidget(LuaLogger& logger);

    void OnMenuItemOpenedFirstTime() override;
    void OnMenuItemJustOpened() override;
    MenuWidgetAction ProcessMenuItem(Common::Time elapsedTime) override;
    void OnMenuItemJustClosed() override;

    void OverrideFunction(sol::string_view key, sol::function function, sol::this_state state);
    void ClearFunctions();

    void SetName(std::string_view name) { _name = name; }
    std::string_view GetName() const { return _name; }

private:
    static constexpr auto KeyOnFirstTimeOpen = "onFirstTimeOpen"sv;
    static constexpr auto KeyOnJustOpen = "onJustOpen"sv;
    static constexpr auto KeyOnUpdate = "onUpdate"sv;
    static constexpr auto KeyOnClosed = "onClosed"sv;

    sol::function _onFirstTimeOpen;
    sol::function _onJustOpened;
    sol::function _onUpdate;
    sol::function _onClosed;
    gsl::not_null<LuaLogger*> _logger;
    std::string_view _name;
};
