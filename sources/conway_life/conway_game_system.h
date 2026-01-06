#pragma once
#include "conway_game.h"
#include "simulation/simulation_player.h"
#include "simulation/simulation_system.h"

class WorldRasterizationTarget;
class ConwayGame;

class ConwayGameSystem final : public SimulationSystem {
public:
    ConwayGameSystem(ConwayGame& game, WorldRasterizationTarget& rasterizationTarget, SimulationPlayer& simulationPlayer);

    void Restart();

    void DoSystemUpdate() override;

    const ConwayGameSummary& GetGameSummary() { return _summary; }

    ConwayGame* ModifyGame();
    const ConwayGame* GetGame() const { return _game; }

private:
    SimulationPlayer* _simulationPlayer{nullptr};
    ConwayGame* _game { nullptr };
    WorldRasterizationTarget* _rasterizationTarget { nullptr };
    ConwayGameSummary _summary;
    bool _inProgress { true };
};