#include "sol/sol.hpp"

#include "application_storage.h"
#include "bind/lua_menu_widget.h"
#include "lua_registrable_system.h"
#include "registrar/registrar.h"
#include "simulation_config.h"
#include "simulation_registrable_system.h"
#include "system/lua_system.h"
#include "system/ui_system.h"
#include "ui_config.h"
#include "ui_registrable_system.h"
#include "widgets/menu_root_widget.h"

namespace {
class ScriptsFixture : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    LuaSystem* luaSystem { nullptr };
    UiSystem* uiSystem { nullptr };

private:
    common::Registrar registrar;
};

class TestConfigurationRegistrableSystem final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;
};

void ScriptsFixture::SetUp()
{
    registrar.Register<TestConfigurationRegistrableSystem>();
    registrar.Register<SimulationRegistrableSystem>();
    UiRegistrableSystem& uiRegistrableSystem = registrar.Register<UiRegistrableSystem>();
    LuaRegistrableSystem& luaRegistrableSystem = registrar.Register<LuaRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        GTEST_FAIL() << std::format("Failed to initialize systems: {}", error.message()) << '\n';
    }

    luaSystem = &luaRegistrableSystem.ModifyLuaSystem();
    uiSystem = &uiRegistrableSystem.ModifyUiSystem();
}

void ScriptsFixture::TearDown()
{
    registrar.RunTerm();
}

std::error_code TestConfigurationRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    auto& simulationConfig = storage.Store<SimulationConfig>();
    simulationConfig.cellsCountX = 1;
    simulationConfig.cellsCountY = 1;

    auto& uiConfig = storage.Store<UiConfig>();
    uiConfig.windowSizeX = 800;
    uiConfig.windowSizeY = 600;
    uiConfig.worldWidgetOffsetX = 10;
    uiConfig.worldWidgetOffsetY = 10;
    uiConfig.worldWidgetSizeX = 50;
    uiConfig.worldWidgetSizeY = 50;
    uiConfig.cellPixelsSize = 10;

    auto& luaConfig = storage.Store<LuaRegistrableSystem::Config>();
    static constexpr auto RelativePathToLuaDirectory = "../../../../sources/tests/lua/"sv;
    luaConfig.luaDirectory = std::filesystem::current_path() / RelativePathToLuaDirectory;

    storage.Store<LuaLogger>([] {
        GTEST_FAIL() << "Script execution error! See details in log";
    });
    return {};
}

void TestConfigurationRegistrableSystem::TerminateSystem()
{
}

}

TEST_F(ScriptsFixture, Widget)
{
    static constexpr auto ScriptName = "test_widget.lua"sv;
    luaSystem->LoadScript(ScriptName);
    sol::function_result testWidget = luaSystem->RunScript(ScriptName);
    ASSERT_TRUE(testWidget.valid());
    const LuaMenuWidget* widget = testWidget;
    ASSERT_NE(widget->GetId(), MenuWidgetId::Invalid);
    ASSERT_EQ(widget->GetName(), "my_test_widget");
    MenuRootWidget& menuRootWidget = uiSystem->ModifyMenuRootWidget();
    ASSERT_TRUE(menuRootWidget.HasWidget(widget->GetId()));
    menuRootWidget.OpenWidget(widget->GetId());
    uiSystem->Update(Common::Time::FromMilliseconds(15));
    menuRootWidget.CloseWidget(widget->GetId());
}
