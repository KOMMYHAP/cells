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
#include "systems/generated/auto_make_emitters_update_random_system.h"
#include "systems/generated/auto_make_emitters_create_new_particles_system.h"
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
    fireworks.emitterFramesToLive = std::numeric_limits<int32_t>::max();
    fireworks.emitterParticlesPerSeconds = 1000;
    fireworks.minAngle = -45.0f;
    fireworks.maxAngle = +45.0f;
    fireworks.minSpeed = 300.0f;
    fireworks.maxSpeed = 1000.0f;
    fireworks.framesToLive = 200;
    fireworks.colorFrom = SDL_Color{ 255, 0, 0, SDL_ALPHA_OPAQUE };
    fireworks.colorTo = SDL_Color{ 255, 255, 0, SDL_ALPHA_OPAQUE };

    auto uiSystem = std::make_unique<UiSystem>(gameConfig.ui);
    EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();
    GameController& gameController = simulationStorage.Store<GameController>();
    gameController.SetGravityEnabled(false);
    GravityUnit& gravityUnit = gameController.ModifyGravityUnit();
    gravityUnit.positionX = 4'000'000.0;
    gravityUnit.positionY = 4'000'000.0;
    gravityUnit.mass = 5.972e24;
    gravityUnit.constant = 6.67e-11;

    uiSystem->ModifyMenuRootWidget().AddWidget<GameWidget>("Game", ecsWorld, gameController, gameConfig);

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

    world->AddSimulationSystem(condAlways, MakeEmittersUpdateLifetimeSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersUpdateRandomSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersInitSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersCreateNewParticlesSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeEmittersUpdateStatsSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeGameControllerUpdateSystem(simulationStorage));

    world->AddSimulationSystem(condAlways, MakeParticlesUpdateLifetimeSystem(simulationStorage));
    world->AddSimulationSystem(condGravity, MakeParticlesUpdateGravitySystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdatePositionSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesDestroyOutOfWorldSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdateColorSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesInitSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesUpdateStatsSystem(simulationStorage));

    world->AddSimulationSystem(condAlways, MakeWorldRasterizationLockSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeParticlesDrawSystem(simulationStorage));
    world->AddSimulationSystem(condAlways, MakeWorldRasterizationUnlockSystem(simulationStorage));

    // const EcsEntity emitter = ecsWorld.create();
    // ecsWorld.emplace<EmitterInitRequest>(emitter, 0, 500.0f, 50.0f);
    uiSystem->ModifyRootWidget().AddWidget(std::move(worldWidget));

    uiSystem->ApplicationRunMainLoop();

    return 0;
}
