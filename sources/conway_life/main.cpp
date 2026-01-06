#include "conway_debug_widget.h"
#include "conway_game.h"
#include "conway_game_system.h"
#include "field_position.h"
#include "menu_widgets/base/group_menu_widget.h"
#include "menu_widgets/engine_summary_widget.h"
#include "menu_widgets/simulation_player_widget.h"
#include "system/ui_system.h"
#include "widgets/menu_root_widget.h"
#include "widgets/world/world_rasterization_target.h"
#include "widgets/world/world_widget.h"
#include "world_statistics.h"

int main()
{
    std::array startup = {
        FieldPosition { 0, 0 },
        FieldPosition { 1, 0 },
        FieldPosition { 0, 1 },
    };

    static constexpr uint16_t CellsCountX = 256;
    static constexpr uint16_t CellsCountY = 128;
    static constexpr uint16_t CellPixelsSize = 4;

    static constexpr uint16_t WindowSizeLimitX = 1200;
    static constexpr uint16_t WindowSizeLimitY = 800;

    static constexpr uint16_t WorldWidgetOffsetX = 20;
    static constexpr uint16_t WorldWidgetOffsetY = 36;

    static constexpr uint16_t WorldWidgetSizeX = CellsCountX * CellPixelsSize;
    static constexpr uint16_t WorldWidgetSizeY = CellsCountY * CellPixelsSize;

    static constexpr uint16_t CellsCountLimitX = (WindowSizeLimitX - 2 * WorldWidgetOffsetX) / CellPixelsSize;
    static_assert(CellsCountX <= CellsCountLimitX, "Maximum cells count by X reached!");

    static constexpr uint16_t CellsCountLimitY = (WindowSizeLimitY - 2 * WorldWidgetOffsetY) / CellPixelsSize;
    static_assert(CellsCountY <= CellsCountLimitY, "Maximum cells count by Y reached!");

    static constexpr uint16_t EstimatedWindowSizeX = WorldWidgetSizeX + 2 * WorldWidgetOffsetX;
    static constexpr uint16_t EstimatedWindowSizeY = WorldWidgetSizeY + 2 * WorldWidgetOffsetY;

    Field field { CellsCountX, CellsCountY };
    for (FieldPosition position : startup) {
        field.Set(position, true);
    }

    ConwayGame game { std::move(field) };

    UiConfig uiConfig;
    uiConfig.windowSizeX = EstimatedWindowSizeX;
    uiConfig.windowSizeY = EstimatedWindowSizeY;
    uiConfig.worldWidgetOffsetX = WorldWidgetOffsetX;
    uiConfig.worldWidgetOffsetY = WorldWidgetOffsetY;
    uiConfig.worldWidgetSizeX = WorldWidgetSizeX;
    uiConfig.worldWidgetSizeY = WorldWidgetSizeY;
    uiConfig.cellPixelsSize = CellPixelsSize;

    auto uiSystem = std::make_unique<UiSystem>(uiConfig);

    const SDL_FRect worldWidgetRect {
        static_cast<float>(uiConfig.worldWidgetOffsetX),
        static_cast<float>(uiConfig.worldWidgetOffsetY),
        static_cast<float>(uiConfig.worldWidgetSizeX),
        static_cast<float>(uiConfig.worldWidgetSizeY),
    };
    auto worldStatistics = std::make_unique<WorldStatistics>();
    auto world = std::make_unique<World>(*worldStatistics);
    world->SetPhase(World::Phase::Running);
    auto worldWidget = uiSystem->MakeWorldWidget(*world, worldWidgetRect);
    auto rasterizationTarget = std::make_unique<WorldRasterizationTarget>(worldWidget->AccessRasterizationTexture(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE }, uiConfig.cellPixelsSize);
    uiSystem->ModifyRootWidget().AddWidget(std::move(worldWidget));

    auto gameSystem = std::make_unique<ConwayGameSystem>(game, *rasterizationTarget, world->ModifySimulationPlayer());
    uiSystem->ModifyMenuRootWidget().AddWidget<EngineSummaryWidget>("Engine", uiSystem->GetAppStatistics());
    auto [simulationGroup, _] = uiSystem->ModifyMenuRootWidget().AddWidget<GroupMenuWidget>("Simulation");
    uiSystem->ModifyMenuRootWidget().AddWidget<SimulationPlayerWidget>(simulationGroup, "Player Control", world->ModifySimulationPlayer());
    uiSystem->ModifyMenuRootWidget().AddWidget<ConwayDebugWidget>(simulationGroup, "Conway", uiConfig, *gameSystem);
    const ConwayGameSummary* summary = &gameSystem->GetGameSummary();
    world->AddSimulationSystem(World::Phase::Running, std::move(gameSystem));

    uiSystem->ApplicationRunMainLoop();

    std::println("Game finished at {} generation!", game.GetCurrentGenerationNumber());

    switch (summary->state) {
    case ConwayGenerationState::NoCellsLeft:
        std::println("Finish reason: no alive cells left!");
        break;
    case ConwayGenerationState::SameField:
        std::println("Finish reason: same field was already generated at {} generation!", summary->similarGenerationNumber.value());
        break;
    default:
        break;
    }

    return 0;
}
