#pragma once

#include "cell_factories/random_cell_factory.h"
#include "simulation/simulation_statistics_provider.h"
#include "simulation/simulation_system.h"
#include "simulation/spawner.h"

class KeepPopulationSystem final : public SimulationSystem {
public:
    struct Config {
        gsl::not_null<EcsWorld*> world;
        gsl::not_null<CellLocator*> locator;
        gsl::not_null<Spawner*> spawner;
        gsl::not_null<RandomCellFactory*> factory;
        gsl::not_null<SimulationStatisticsProvider*> stats;
        gsl::not_null<Random::Engine*> random;
    };
    explicit KeepPopulationSystem(const Config& config);

    void DoSystemUpdate() override;

private:
    void Spawn(CellPosition position);

    Config _config;
};
