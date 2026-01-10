#include "world.h"
#include "world_statistics.h"

#include "components/generated/auto_particle_lifetime.h"
#include "game_config.h"
#include "system/ui_system.h"

int main()
{
    auto worldStatistics = std::make_unique<WorldStatistics>();
    auto world = std::make_unique<World>(*worldStatistics);

    GameConfig gameConfig;
    static constexpr GameUiConfig<1000, 700, 1, 1200, 800, 16, 24> GameUiConfigInstance {};
    gameConfig.ui = GameUiConfigInstance.ToUiConfig();
    auto uiSystem = std::make_unique<UiSystem>(gameConfig.ui);

    SimulationStorage& simulationStorage = world->ModifySimulation();
    EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();

    return 0;
}
