#pragma once
#include "game_config.h"
#include "game_controller.h"
#include "menu_widgets/base/base_menu_widget.h"
#include "simulation/simulation_ecs_config.h"

class GameController;
class GameWidget final : public BaseMenuWidget {
public:
    GameWidget(EcsWorld& ecsWorld, GameController& gameController, GameConfig& gameConfig);
    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    struct EmitterData {
        EcsEntity emitter { InvalidEcsEntity };
        float position[2] { 0.0f, 0.0f };
        int32_t generation { 0 };
        bool shouldRemove{false};
        bool shouldRecreate{false};
    };
    struct GenerationData {
        GameConfig::FireworksConfig config;
        bool shouldRemove{false};
    };

    void ProcessEmitters();
    void ProcessEmitterEntry(EmitterData & emitterData);

    void ProcessGenerations();
    void ProcessGenerationEntry(GenerationData& generationData);

    void ProcessGravity(GravityUnit& gravity);

    EcsWorld* _ecsWorld { nullptr };
    GameController* _gameController { nullptr };
    GameConfig* _gameConfig { nullptr };
    std::vector<EmitterData> _emitters;
    std::vector<GenerationData> _generations;
};