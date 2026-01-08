#include "basic_defines.h"
#include "simulation_config.h"

#include "registrar/registrar.h"

#include "menu_widgets/base/group_menu_widget.h"
#include "menu_widgets/engine_summary_widget.h"
#include "menu_widgets/simulation_summary_widget.h"
#include "procedures/look_procedure_system.h"
#include "procedures/move_procedure_system.h"
#include "procedures/random_cell_spawn_procedure_system.h"
#include "simulation_registrable_system.h"
#include "simulation/simulation_statistics_provider.h"
#include "system/ui_system.h"
#include "systems_ecs/generated/auto_death_from_age_statistics_system.h"
#include "systems_ecs/generated/auto_make_age_system.h"
#include "systems_ecs/generated/auto_make_alive_cells_statistics_system.h"
#include "systems_ecs/generated/auto_make_brain_simulation_system.h"
#include "systems_ecs/generated/auto_make_death_from_age_statistics_system.h"
#include "systems_ecs/generated/auto_make_death_from_empty_energy_statistics_system.h"
#include "systems_ecs/generated/auto_make_energy_decrease_system.h"
#include "systems_ecs/generated/auto_make_energy_leak_system.h"
#include "systems_ecs/generated/auto_make_graveyard_system.h"
#include "systems_ecs/generated/auto_make_keep_population_system.h"
#include "systems_ecs/generated/auto_make_spawn_places_statistics_system.h"
#include "systems_ecs/generated/auto_make_spawn_system.h"
#include "systems_ecs/generated/auto_make_world_rasterization_lock_system.h"
#include "systems_ecs/generated/auto_make_world_rasterization_system.h"
#include "systems_ecs/generated/auto_make_world_rasterization_unlock_system.h"
#include "ui_config.h"
#include "ui_registrable_system.h"
#include "widgets/menu_root_widget.h"
#include "widgets/world/world_rasterization_target.h"
#include "widgets/world/world_widget.h"
#include "world.h"
#include "world_statistics.h"

namespace {

class ConfigurationRegistrableSystem final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override { }
};

class WorldSetupRegistrableSystem final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(ApplicationStorage& applicationStorage) override;
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

    storage.Store<const SimulationConfig>(CellsCountX, CellsCountY);

    UiConfig uiConfig;
    uiConfig.windowSizeX = EstimatedWindowSizeX;
    uiConfig.windowSizeY = EstimatedWindowSizeY;
    uiConfig.worldWidgetOffsetX = WorldWidgetOffset;
    uiConfig.worldWidgetOffsetY = WorldWidgetOffset;
    uiConfig.worldWidgetSizeX = WorldWidgetSizeX;
    uiConfig.worldWidgetSizeY = WorldWidgetSizeY;
    uiConfig.cellPixelsSize = CellPixelsSize;
    storage.Store<const UiConfig>(uiConfig);

    // auto& luaConfig = storage.Store<LuaRegistrableSystem::Config>();
    // static constexpr auto RelativePathToLuaDirectory = "../../../../sources/scripts/lua/"sv;
    // luaConfig.luaDirectory = std::filesystem::current_path() / RelativePathToLuaDirectory;
    // luaConfig.startupScript = "loader.lua"sv;

    return {};
}

template <class T, class... Args>
    requires std::is_base_of_v<ProcedureBase, T> && std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
void TEMP_RegisterProcedureSystem(World& world, ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
{
    SimulationStorage& storage = world.ModifySimulation();
    SimulationVirtualMachine& vm = storage.Modify<SimulationVirtualMachine>();

    auto procedure = std::make_unique<T>(std::forward<Args>(args)...);
    T* weakProcedure = procedure.get();
    vm.RegisterProcedure(type, weakProcedure, inputCount, outputCount, std::move(name));

    world.AddSimulationSystem(World::Phase::Running, std::move(procedure));
}

std::error_code WorldSetupRegistrableSystem::InitializeSystem(ApplicationStorage& applicationStorage)
{
    World& world = applicationStorage.Modify<World>();
    const SimulationConfig& simulationConfig = applicationStorage.Get<const SimulationConfig>();
    const UiConfig& uiConfig = applicationStorage.Get<const UiConfig>();
    UiSystem& uiSystem = applicationStorage.Modify<UiSystem>();
    SimulationStorage& simulationStorage = world.ModifySimulation();
    EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();

    const SDL_FRect worldWidgetRect {
        static_cast<float>(uiConfig.worldWidgetOffsetX),
        static_cast<float>(uiConfig.worldWidgetOffsetY),
        static_cast<float>(uiConfig.worldWidgetSizeX),
        static_cast<float>(uiConfig.worldWidgetSizeY),
    };
    std::unique_ptr<WorldWidget> worldWidget = uiSystem.MakeWorldWidget(world, worldWidgetRect);
    simulationStorage.Store<WorldRasterizationTarget>(worldWidget->AccessRasterizationTexture(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE }, uiConfig.cellPixelsSize);
    uiSystem.ModifyRootWidget().AddWidget(std::move(worldWidget));

    CellLocator& cellLocator = simulationStorage.Store<CellLocator>(simulationConfig.cellsCountX, simulationConfig.cellsCountY);
    Spawner& spawner = simulationStorage.Store<Spawner>(ecsWorld, cellLocator);
    SimulationVirtualMachine& vm = simulationStorage.Store<SimulationVirtualMachine>(ecsWorld);
    Random::Engine& random = simulationStorage.Store<Random::Engine>(Random::MakeEngine("42"sv));
    RandomCellFactory& randomCellFactory = simulationStorage.Store<RandomCellFactory>(vm, random);
    SimulationStatisticsProvider& simulationStats = simulationStorage.Store<SimulationStatisticsProvider>(cellLocator);

    using EcsSystemFactory = std::unique_ptr<SimulationSystem> (*)(const SimulationStorage&);
    auto AddSimulation = [&simulationStorage, &world](World::Phase phase, const EcsSystemFactory& factory) {
        std::unique_ptr<SimulationSystem> system = factory(simulationStorage);
        world.AddSimulationSystem(phase, std::move(system));
    };

    AddSimulation(World::Phase::Running, &MakeSpawnSystem);
    AddSimulation(World::Phase::Running, &MakeBrainSimulationSystem);
    TEMP_RegisterProcedureSystem<RandomCellSpawnProcedureSystem>(world, ProcedureType::SpawnRandomCell, 1, 0, "SpawnRandomCell", ecsWorld, vm, cellLocator, spawner, randomCellFactory);
    AddSimulation(World::Phase::Running, &MakeEnergyLeakSystem);
    AddSimulation(World::Phase::Running, &MakeEnergyDecreaseSystem);
    AddSimulation(World::Phase::Running, &MakeAgeSystem);
    TEMP_RegisterProcedureSystem<LookProcedureSystem>(world, ProcedureType::Look, 1, 1, "Look", ecsWorld, vm, cellLocator);
    TEMP_RegisterProcedureSystem<MoveProcedureSystem>(world, ProcedureType::Move, 1, 0, "Move", ecsWorld, vm, cellLocator);
    AddSimulation(World::Phase::Running, &MakeAliveCellsStatisticsSystem);
    AddSimulation(World::Phase::Running, &MakeSpawnPlacesStatisticsSystem);
    AddSimulation(World::Phase::Running, &MakeDeathFromAgeStatisticsSystem);
    AddSimulation(World::Phase::Running, &MakeDeathFromEmptyEnergyStatisticsSystem);
    AddSimulation(World::Phase::Running, &MakeGraveyardSystem);
    AddSimulation(World::Phase::Stopped, &MakeKeepPopulationSystem);
    AddSimulation(World::Phase::Running, &MakeWorldRasterizationLockSystem);
    AddSimulation(World::Phase::Running, &MakeWorldRasterizationSystem);
    AddSimulation(World::Phase::Running, &MakeWorldRasterizationUnlockSystem);

    {
        MenuRootWidget& menuRoot = uiSystem.ModifyMenuRootWidget();
        auto [engineRootId, _1] = menuRoot.AddWidget<GroupMenuWidget>("Engine");
        menuRoot.AddWidget<EngineSummaryWidget>(engineRootId, "Summary", uiSystem.GetAppStatistics());

        auto [simulationRootId, _2] = menuRoot.AddWidget<GroupMenuWidget>("Simulation");
        menuRoot.AddWidget<SimulationSummaryWidget>(simulationRootId, "Summary", world, applicationStorage.Get<WorldStatistics>(), simulationStats);
    }

    return {};
}

}

int main(int /*argc*/, char** /*argv*/)
{
    common::Registrar registrar;
    registrar.Register<ConfigurationRegistrableSystem>();
    registrar.Register<SimulationRegistrableSystem>();
    const UiRegistrableSystem& uiSystem = registrar.Register<UiRegistrableSystem>();
    // registrar.Register<LuaRegistrableSystem>();
    registrar.Register<WorldSetupRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return -1;
    }

    uiSystem.GetUiApplicationInterface()->ApplicationRunMainLoop();

    registrar.RunTerm();
    return 0;
}
