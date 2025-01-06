#include "basic_defines.h"
#include "simulation_config.h"

#include "registrar/registrar.h"

#include "lua_registrable_system.h"
#include "simulation_registrable_system.h"
#include "ui_config.h"
#include "ui_registrable_system.h"

namespace {

class ConfigurationRegistrableSystem final : public common::RegistrableSystem {
public:
    ConfigurationRegistrableSystem() = default;
    ConfigurationRegistrableSystem(const ConfigurationRegistrableSystem& other) = delete;
    ConfigurationRegistrableSystem(ConfigurationRegistrableSystem&& other) noexcept = delete;
    ConfigurationRegistrableSystem& operator=(const ConfigurationRegistrableSystem& other) = delete;
    ConfigurationRegistrableSystem& operator=(ConfigurationRegistrableSystem&& other) noexcept = delete;
    ~ConfigurationRegistrableSystem() override = default;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override { }
};

std::error_code ConfigurationRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    static constexpr uint16_t CellsCountX = 190;
    static constexpr uint16_t CellsCountY = 140;
    static constexpr uint16_t CellPixelsSize = 4;

    static constexpr uint16_t WindowSizeLimitX = 800;
    static constexpr uint16_t WindowSizeLimitY = 600;

    static constexpr uint16_t WorldWidgetOffset = 20;
    static constexpr uint16_t WorldWidgetSizeX = CellsCountX * CellPixelsSize;
    static constexpr uint16_t WorldWidgetSizeY = CellsCountY * CellPixelsSize;

    static constexpr uint16_t CellsCountLimitX = (WindowSizeLimitX - 2 * WorldWidgetOffset) / CellPixelsSize;
    static_assert(CellsCountX <= CellsCountLimitX, "Maximum cells count by X reached!");

    static constexpr uint16_t CellsCountLimitY = (WindowSizeLimitY - 2 * WorldWidgetOffset) / CellPixelsSize;
    static_assert(CellsCountY <= CellsCountLimitY, "Maximum cells count by Y reached!");

    static constexpr uint16_t EstimatedWindowSizeX = WorldWidgetSizeX + 2 * WorldWidgetOffset;
    static constexpr uint16_t EstimatedWindowSizeY = WorldWidgetSizeY + 2 * WorldWidgetOffset;

    auto& simulationConfig = storage.Store<SimulationConfig>();
    simulationConfig.cellsCountX = CellsCountX;
    simulationConfig.cellsCountY = CellsCountY;

    auto& uiConfig = storage.Store<UiConfig>();
    uiConfig.windowSizeX = EstimatedWindowSizeX;
    uiConfig.windowSizeY = EstimatedWindowSizeY;
    uiConfig.worldWidgetOffsetX = WorldWidgetOffset;
    uiConfig.worldWidgetOffsetY = WorldWidgetOffset;
    uiConfig.worldWidgetSizeX = WorldWidgetSizeX;
    uiConfig.worldWidgetSizeY = WorldWidgetSizeY;
    uiConfig.cellPixelsSize = CellPixelsSize;

    auto& luaConfig = storage.Store<LuaRegistrableSystem::Config>();
    static constexpr auto RelativePathToLuaDirectory = "../../../../sources/scripts/lua/"sv;
    luaConfig.luaDirectory = std::filesystem::current_path() / RelativePathToLuaDirectory;
    luaConfig.startupScript = "loader.lua"sv;

    return {};
}
}

int main(int /*argc*/, char** /*argv*/)
{
    common::Registrar registrar;
    registrar.Register<ConfigurationRegistrableSystem>();
    registrar.Register<SimulationRegistrableSystem>();
    const UiRegistrableSystem& uiSystem = registrar.Register<UiRegistrableSystem>();
    registrar.Register<LuaRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return -1;
    }

    uiSystem.GetUiApplicationInterface()->ApplicationRunMainLoop();

    registrar.RunTerm();
    return 0;
}
