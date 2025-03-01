#include "basic_defines.h"
#include "simulation_config.h"

#include "registrar/registrar.h"

#include "lua_registrable_system.h"
#include "procedures/look_procedure_system.h"
#include "procedures/move_procedure_system.h"
#include "procedures/random_cell_spawn_procedure_system.h"
#include "simulation_registrable_system.h"
#include "system/ui_system.h"
#include "systems_ecs/alive_cells_statistics_system.h"
#include "systems_ecs/brain_simulation_system.h"
#include "systems_ecs/death_from_age_statistics_system.h"
#include "systems_ecs/death_from_empty_energy_statistics_system.h"
#include "systems_ecs/energy_decrease_system.h"
#include "systems_ecs/energy_leak_system.h"
#include "systems_ecs/generated/auto_make_age_system.h"
#include "systems_ecs/graveyard_system.h"
#include "systems_ecs/keep_population_system.h"
#include "systems_ecs/spawn_places_statistics_system.h"
#include "systems_ecs/spawn_system.h"
#include "ui_config.h"
#include "ui_registrable_system.h"
#include "widgets/world/world_rasterization_system.h"
#include "widgets/world/world_widget.h"
#include "world.h"

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

    auto& luaConfig = storage.Store<LuaRegistrableSystem::Config>();
    static constexpr auto RelativePathToLuaDirectory = "../../../../sources/scripts/lua/"sv;
    luaConfig.luaDirectory = std::filesystem::current_path() / RelativePathToLuaDirectory;
    luaConfig.startupScript = "loader.lua"sv;

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

    world.AddSimulationSystem(std::move(procedure));
}

template <class T, class... Args>
    requires std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
void TEMP_RegisterSystem(World& world, Args&&... args)
{
    auto system = std::make_unique<T>(std::forward<Args>(args)...);
    world.AddSimulationSystem(std::move(system));
}

std::error_code WorldSetupRegistrableSystem::InitializeSystem(ApplicationStorage& applicationStorage)
{
    World& world = applicationStorage.Modify<World>();
    const SimulationConfig& simulationConfig = applicationStorage.Get<const SimulationConfig>();
    const UiConfig& uiConfig = applicationStorage.Get<const UiConfig>();
    UiSystem& uiSystem = applicationStorage.Modify<UiSystem>();
    SimulationStorage& simulationStorage = world.ModifySimulation();
    EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();

    CellLocator& cellLocator = simulationStorage.Store<CellLocator>(simulationConfig.cellsCountX, simulationConfig.cellsCountY);
    Spawner& spawner = simulationStorage.Store<Spawner>(ecsWorld, cellLocator);
    SimulationVirtualMachine& vm = simulationStorage.Store<SimulationVirtualMachine>(ecsWorld);
    Random::Engine& random = simulationStorage.Store<Random::Engine>(Random::MakeEngine("42"sv));
    RandomCellFactory& randomCellFactory = simulationStorage.Store<RandomCellFactory>(vm, random);
    SimulationStatisticsProvider& statistics = simulationStorage.Store<SimulationStatisticsProvider>(cellLocator);

    using EcsSystemFactory = std::unique_ptr<SimulationSystem> (*)(const SimulationStorage&);
    auto RegisterEcsSystem = [&simulationStorage, &world](const EcsSystemFactory& factory) {
        std::unique_ptr<SimulationSystem> system = factory(simulationStorage);
        world.AddSimulationSystem(std::move(system));
    };

    TEMP_RegisterSystem<SpawnSystem>(world, ecsWorld, cellLocator);
    TEMP_RegisterSystem<BrainSimulationSystem>(world, ecsWorld, vm);
    TEMP_RegisterProcedureSystem<RandomCellSpawnProcedureSystem>(world, ProcedureType::SpawnRandomCell, 1, 0, "SpawnRandomCell", ecsWorld, vm, cellLocator, spawner, randomCellFactory);
    TEMP_RegisterSystem<EnergyLeakSystem>(world, ecsWorld);
    TEMP_RegisterSystem<EnergyDecreaseSystem>(world, ecsWorld);
    RegisterEcsSystem(&MakeAgeSystem);
    TEMP_RegisterProcedureSystem<LookProcedureSystem>(world, ProcedureType::Look, 1, 1, "Look", ecsWorld, vm, cellLocator);
    TEMP_RegisterProcedureSystem<MoveProcedureSystem>(world, ProcedureType::Move, 1, 0, "Move", ecsWorld, vm, cellLocator);
    TEMP_RegisterSystem<AliveCellsStatisticsSystem>(world, ecsWorld, statistics);
    TEMP_RegisterSystem<SpawnPlacesStatisticsSystem>(world, ecsWorld, statistics);
    TEMP_RegisterSystem<DeathFromAgeStatisticsSystem>(world, ecsWorld, statistics);
    TEMP_RegisterSystem<DeathFromEmptyEnergySystem>(world, ecsWorld, statistics);
    TEMP_RegisterSystem<GraveyardSystem>(world, ecsWorld, cellLocator);
    {
        KeepPopulationSystem::Config config {
            &ecsWorld,
            &cellLocator,
            &spawner,
            &randomCellFactory,
            &statistics,
            &random
        };
        TEMP_RegisterSystem<KeepPopulationSystem>(world, config);
    }

    auto worldRasterizationSystem = std::make_unique<WorldRasterizationSystem>(ecsWorld, uiConfig.cellPixelsSize);
    std::unique_ptr<WorldWidget> worldWidget = uiSystem.MakeWorldWidget(world, *worldRasterizationSystem, uiConfig.worldWidgetOffsetX, uiConfig.worldWidgetOffsetY, uiConfig.worldWidgetSizeX, uiConfig.worldWidgetSizeY);
    uiSystem.ModifyRootWidget().AddWidget(std::move(worldWidget));
    world.AddSimulationSystem(std::move(worldRasterizationSystem));

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
    registrar.Register<WorldSetupRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return -1;
    }

    uiSystem.GetUiApplicationInterface()->ApplicationRunMainLoop();

    registrar.RunTerm();
    return 0;
}
