#include "application_storage.h"
#include "lua_registrable_system.h"
#include "registrar/registrar.h"
#include "simulation_config.h"
#include "simulation_registrable_system.h"
#include "system/lua_system.h"
#include "ui_config.h"
#include "ui_registrable_system.h"

namespace {
class ScriptsFixture : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    void Run(std::string_view filename);

private:
    std::optional<std::string> LoadScript(const std::filesystem::path& path);

    common::Registrar registrar;
    LuaSystem* luaSystem { nullptr };
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
    const UiRegistrableSystem& uiSystem = registrar.Register<UiRegistrableSystem>();
    LuaRegistrableSystem& luaRegistrableSystem = registrar.Register<LuaRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        ASSERT_EQ(error, std::error_code {}) << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return;
    }

    luaSystem = &luaRegistrableSystem.ModifyLuaSystem();
    uiSystem.GetUiApplicationInterface()->ApplicationRunMainLoop();
}

void ScriptsFixture::TearDown()
{
    registrar.RunTerm();
}

void ScriptsFixture::Run(std::string_view filename)
{
    const std::filesystem::path path = std::filesystem::path { "../../sources/tests/lua" } / filename;
    const std::optional<std::string> mbData = LoadScript(path);
    ASSERT_TRUE(mbData) << std::format("Cannot load script {}", filename);
    luaSystem->RunScript(*mbData);
}

std::optional<std::string> ScriptsFixture::LoadScript(const std::filesystem::path& path)
{
    std::ifstream ifs { path };
    if (!ifs.is_open()) {
        return {};
    }
    ifs.seekg(0, std::ios::end);
    const std::streampos size = ifs.tellg();
    ifs.seekg(0, std::ios::cur);
    std::string data { size + std::streampos { 1 } };
    ifs.read(data.data(), data.length());
    return data;
}

std::error_code TestConfigurationRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    auto& simulationConfig = storage.Store<SimulationConfig>();
    simulationConfig.cellsCountX = 1;
    simulationConfig.cellsCountY = 1;

    auto& uiConfig = storage.Store<UiConfig>();
    uiConfig.windowSizeX = 100;
    uiConfig.windowSizeY = 100;
    uiConfig.worldWidgetOffsetX = 10;
    uiConfig.worldWidgetOffsetY = 10;
    uiConfig.worldWidgetSizeX = 50;
    uiConfig.worldWidgetSizeY = 50;
    uiConfig.cellPixelsSize = 10;

    storage.Store<LuaLogger>([this] {
        ASSERT_TRUE(false, "Script exection error! See details in log");
    });
    return {};
}

void TestConfigurationRegistrableSystem::TerminateSystem()
{
}

}

TEST_F(ScriptsFixture, Widget)
{
    Run("test_widget.lua");
}