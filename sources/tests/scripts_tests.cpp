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

    sol::function_result Run(std::string_view filename);
    UiSystem& ModifyUiSystem();

private:
    std::optional<std::string> LoadScript(const std::filesystem::path& path);

    common::Registrar registrar;
    LuaSystem* luaSystem { nullptr };
    UiSystem* uiSystem { nullptr };
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
        return;
    }

    luaSystem = &luaRegistrableSystem.ModifyLuaSystem();
    uiSystem = &uiRegistrableSystem.ModifyUiSystem();
}

void ScriptsFixture::TearDown()
{
    registrar.RunTerm();
}

sol::function_result ScriptsFixture::Run(std::string_view filename)
{
    const std::filesystem::path relativePathToLuaTestsDirectory { "../../../../sources/tests/lua" };
    const std::filesystem::path path = std::filesystem::current_path() / relativePathToLuaTestsDirectory / filename;
    const std::optional<std::string> mbData = LoadScript(path);
    if (!mbData) {
        return {};
    }
    sol::function_result result = luaSystem->RunScript(*mbData);
    return result;
}

UiSystem& ScriptsFixture::ModifyUiSystem()
{
    return *uiSystem;
}

std::optional<std::string> ScriptsFixture::LoadScript(const std::filesystem::path& path)
{
    std::ifstream ifs { path };
    if (!ifs.is_open()) {
        return {};
    }
    ifs.seekg(0, std::ios::end);
    const std::ifstream::pos_type size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    auto data = std::string(size + std::ifstream::off_type { 1 }, '\0');
    ifs.read(data.data(), data.length());
    std::erase_if(data, [](char c) { return c == '\0'; });
    return data;
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
    sol::function_result testWidget = Run("test_widget.lua");
    ASSERT_TRUE(testWidget.valid());
    const LuaMenuWidget* widget = testWidget;
    ASSERT_NE(widget->GetId(), MenuWidgetId::Invalid);
    ASSERT_EQ(widget->GetName(), "my_test_widget");
    UiSystem& uiSystem = ModifyUiSystem();
    MenuRootWidget& menuRootWidget = uiSystem.ModifyMenuRootWidget();
    ASSERT_TRUE(menuRootWidget.HasWidget(widget->GetId()));
    menuRootWidget.OpenWidget(widget->GetId());
    uiSystem.Update(Common::Time::FromMilliseconds(15));
    menuRootWidget.CloseWidget(widget->GetId());
}
