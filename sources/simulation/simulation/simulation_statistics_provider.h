#pragma once

class SpawnPlacesStatisticsSystem;
class AliveCellsStatisticsSystem;
class CellLocator;

class SimulationStatisticsProvider {
public:
    struct Config {
        gsl::not_null<CellLocator*> locator;
        gsl::not_null<AliveCellsStatisticsSystem*> aliveCellsStats;
        gsl::not_null<SpawnPlacesStatisticsSystem*> spawnPlacesStats;
    };
    SimulationStatisticsProvider(const Config& config);

    size_t GetCellsCount() const;
    size_t GetSpawnPlacesCount() const;
    size_t GetCellsCapacity() const;

private:
    Config _config;
};