int main()
{
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
    SimulationStorage& simulationStorage = world->ModifySimulation();
    simulationStorage.Store<UiConfig>(uiConfig);
    simulationStorage.Store<GameConfig>();
    auto worldWidget = uiSystem->MakeWorldWidget(*world, worldWidgetRect);

    simulationStorage.Store<WorldRasterizationTarget>(worldWidget->AccessRasterizationTexture(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE }, uiConfig.cellPixelsSize);
    uiSystem->ModifyRootWidget().AddWidget(std::move(worldWidget));

    CellLocator& cellLocator = simulationStorage.Store<CellLocator>(CellsCountX, CellsCountY);
    {
        EcsWorld& ecsWorld = simulationStorage.Modify<EcsWorld>();
        for (int32_t y = 0; y < CellsCountY; ++y) {
            for (int32_t x = 0; x < CellsCountX; ++x) {
                const EcsEntity entity = ecsWorld.create();
                const CellPosition position { static_cast<int16_t>(x), static_cast<int16_t>(y) };
                cellLocator.Set(position, entity);
            }
        }

        std::ranges::sort(startupPositions);

        for (int32_t y = 0; y < CellsCountY; ++y) {
            for (int32_t x = 0; x < CellsCountX; ++x) {
                const CellPosition position {
                    static_cast<int16_t>(x),
                    static_cast<int16_t>(y)
                };
                const EcsEntity cell = cellLocator.Find(position);

                const bool hasLife = std::ranges::binary_search(startupPositions, position);
                if (hasLife) {
                    ecsWorld.emplace<WillBornTag>(cell);
                }
                ecsWorld.emplace<CellPosition>(cell, position);

                Neighbours neighboursComponent;
                neighboursComponent.entities[Neighbours::N00] = cellLocator.Find(CellPosition { static_cast<int16_t>(x - 1), static_cast<int16_t>(y - 1) });
                neighboursComponent.entities[Neighbours::N10] = cellLocator.Find(CellPosition { static_cast<int16_t>(x - 0), static_cast<int16_t>(y - 1) });
                neighboursComponent.entities[Neighbours::N20] = cellLocator.Find(CellPosition { static_cast<int16_t>(x + 1), static_cast<int16_t>(y - 1) });
                neighboursComponent.entities[Neighbours::N01] = cellLocator.Find(CellPosition { static_cast<int16_t>(x - 1), static_cast<int16_t>(y - 0) });
                neighboursComponent.entities[Neighbours::N21] = cellLocator.Find(CellPosition { static_cast<int16_t>(x + 1), static_cast<int16_t>(y - 0) });
                neighboursComponent.entities[Neighbours::N02] = cellLocator.Find(CellPosition { static_cast<int16_t>(x - 1), static_cast<int16_t>(y + 1) });
                neighboursComponent.entities[Neighbours::N12] = cellLocator.Find(CellPosition { static_cast<int16_t>(x - 0), static_cast<int16_t>(y + 1) });
                neighboursComponent.entities[Neighbours::N22] = cellLocator.Find(CellPosition { static_cast<int16_t>(x + 1), static_cast<int16_t>(y + 1) });

                ecsWorld.emplace<Neighbours>(cell, neighboursComponent);
                ecsWorld.emplace<NeighboursCount>(cell, NeighboursCount {});
            }
        }
    }

    uiSystem->ModifyMenuRootWidget().AddWidget<EngineSummaryWidget>("Engine", uiSystem->GetAppStatistics());
    auto [simulationGroup, _] = uiSystem->ModifyMenuRootWidget().AddWidget<GroupMenuWidget>("Simulation");
    SimulationPlayer& simulationPlayer = world->ModifySimulationPlayer();
    uiSystem->ModifyMenuRootWidget().AddWidget<SimulationPlayerWidget>(simulationGroup, "Player Control", simulationPlayer);

    ConwayGameController& gameController = simulationStorage.Store<ConwayGameController>();
    const Common::Condition conditionGameInProcess = world->ModifyConditionSystem().Register("game_in_process", [&gameController, &simulationPlayer]() -> bool {
        return simulationPlayer.ShouldPlayFrame() && gameController.GetSummary().state == ConwayGenerationState::Good;
    });
    const Common::Condition conditionAlways = world->ModifyConditionSystem().Register("always", []() -> bool {
        return true;
    });
    uiSystem->ModifyMenuRootWidget().AddWidget<ConwayDebugWidget>(simulationGroup, "Conway", simulationStorage);

    world->AddSimulationSystem(conditionGameInProcess, MakeInvalidateNeighboursSystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeCollectNeighboursCountSystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeProcessAliveCellVitalitySystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeProcessEmptyCellVitalitySystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeCreateBirthEmitterSystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeCreateDeathEmitterSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeRemoveDiedCellSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeBornNewLifeSystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeEmitNewParticleSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeUpdateEmitterSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeUpdateParticleSystem(simulationStorage));
    world->AddSimulationSystem(conditionGameInProcess, MakeUpdateGameControllerSystem(simulationStorage));

    world->AddSimulationSystem(conditionAlways, MakeWorldRasterizationLockSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeDrawAliveCellSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeDrawCellParticleSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeDrawEmptyCellSystem(simulationStorage));
    world->AddSimulationSystem(conditionAlways, MakeWorldRasterizationUnlockSystem(simulationStorage));

    simulationPlayer.SetFixedSpeedMode(10.0);
    uiSystem->ApplicationRunMainLoop();

    const ConwayGameSummary finalGameSummary = gameController.GetSummary();
    std::println("Game finished at {} generation!", finalGameSummary.generationNumber);

    switch (finalGameSummary.state) {
    case ConwayGenerationState::NoCellsLeft:
        std::println("Finish reason: no alive cells left!");
        break;
    case ConwayGenerationState::SameField:
        std::println("Finish reason: same field was already generated at {} generation!", *finalGameSummary.similarGenerationNumber);
        break;
    default:
        break;
    }

    return 0;
}
