#include "world.h"
#include "world_statistics.h"

#include "game_config.h"
#include "game_controller.h"
#include "game_widget.h"
#include "system/ui_system.h"

#include "components/generated/auto_emitter_init_request.h"
#include "components/generated/auto_particle_lifetime.h"

#include "systems/generated/auto_make_emitters_init_system.h"
#include "systems/generated/auto_make_emitters_update_lifetime_system.h"
#include "systems/generated/auto_make_emitters_update_stats_system.h"
#include "systems/generated/auto_make_empty_cell_draw_system.h"
#include "systems/generated/auto_make_game_controller_update_system.h"
#include "systems/generated/auto_make_particles_destroy_out_of_world_system.h"
#include "systems/generated/auto_make_particles_draw_system.h"
#include "systems/generated/auto_make_particles_init_system.h"
#include "systems/generated/auto_make_particles_update_color_system.h"
#include "systems/generated/auto_make_particles_update_gravity_system.h"
#include "systems/generated/auto_make_particles_update_lifetime_system.h"
#include "systems/generated/auto_make_particles_update_position_system.h"
#include "systems/generated/auto_make_particles_update_stats_system.h"
#include "systems_ecs/generated/auto_make_world_rasterization_lock_system.h"
#include "systems_ecs/generated/auto_make_world_rasterization_unlock_system.h"
#include "widgets/menu_root_widget.h"
#include "widgets/world/world_rasterization_target.h"
#include "widgets/world/world_widget.h"

int main()
{
    auto world = std::make_unique<World>();
    world->ModifySimulationPlayer().SetFixedSpeedMode(60);
    SimulationStorage& simulationStorage = world->ModifySimulation();

    GameConfig& gameConfig = simulationStorage.Store<GameConfig>();
    static constexpr GameUiConfig<1000, 600, 1, 1200, 800, 16, 24> GameUiConfigInstance {};
    gameConfig.ui = GameUiConfigInstance.ToUiConfig();
    gameConfig.fireworksColors =  {
        SDL_Color { 0, 0, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 0, 255, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 0, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 0, 255, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 255, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 0, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 255, 255, SDL_ALPHA_OPAQUE },
    };

    GameConfig::FireworksConfig & fireworks = gameConfig.ModifyFireworks().emplace_back();
    fireworks.emitterFramesToLive = 10000;
    fireworks.emitterFramesToEmit = 20;
    fireworks.minDirectionX = -0.5f;
    fireworks.maxDirectionX = 0.5f;
    fireworks.minDirectionY = -0.5f;
    fireworks.maxDirectionY = 0.5f;
    fireworks.minSpeed = 10.0f;
    fireworks.maxSpeed = 100.0f;
    fireworks.framesToLive = 100;
    fireworks.colorFrom = SDL_Color{ 0, 0, 0, SDL_ALPHA_OPAQUE };
    fireworks.colorTo = SDL_Color{ 255, 255, 0, SDL_ALPHA_OPAQUE };

    auto uiSystem = std::make_unique<UiSystem>(gameConfig.ui);
    EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();
    GameController& gameController = simulationStorage.Store<GameController>();
    gameController.SetGravityEnabled(false);

    uiSystem->ModifyMenuRootWidget().AddWidget<GameWidget>("Game", gameController, gameConfig);

    const SDL_FRect worldWidgetRect {
        static_cast<float>(gameConfig.ui.worldWidgetOffsetX),
        static_cast<float>(gameConfig.ui.worldWidgetOffsetY),
        static_cast<float>(gameConfig.ui.worldWidgetSizeX),
        static_cast<float>(gameConfig.ui.worldWidgetSizeY),
    };
    auto worldWidget = uiSystem->MakeWorldWidget(*world, worldWidgetRect);
    simulationStorage.Store<WorldRasterizationTarget>(worldWidget->AccessRasterizationTexture(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE }, gameConfig.ui.cellPixelsSize);

    Common::ConditionSystem& conditions = world->ModifyConditionSystem();
    const Common::Condition condAlways = conditions.Register("enabled_always", [] { return true; });
    const Common::Condition condGravity = conditions.Register("gravity", [&gameController] { return gameController.IsGravityEnabled(); });

    world->AddSimulationSystem(condAlways, MakeEmittersInitSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersUpdateLifetimeSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersUpdateStatsSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeGameControllerUpdateSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesInitSystem(simulationStorage));
    world->AddSimulationSystem(condGravity, MakeParticlesUpdateGravitySystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdatePositionSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesDestroyOutOfWorldSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdateLifetimeSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdateColorSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdateStatsSystem(simulationStorage));

    world->AddSimulationSystem(condAlways, MakeWorldRasterizationLockSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesDrawSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeWorldRasterizationUnlockSystem(simulationStorage));

    const EcsEntity emitter = ecsWorld.create();
    ecsWorld.emplace<EmitterInitRequest>(emitter, 0, 500.0f, 50.0f);
    uiSystem->ModifyRootWidget().AddWidget(std::move(worldWidget));

    uiSystem->ApplicationRunMainLoop();

    return 0;
}
