#pragma once

class CellLocator;

class SimulationStatisticsProvider {
public:
    explicit SimulationStatisticsProvider(const CellLocator& locator);

    int64_t GetCellsCapacity() const { return _cellsCapacity; }

    void SetCellsCount(int64_t cellsCount) { _cellsCount = cellsCount; }
    int64_t GetCellsCount() const { return _cellsCount; }

    void SetSpawnPlacesCount(int64_t spawnPlacesCount) { _spawnPlacesCount = spawnPlacesCount; }
    int64_t GetSpawnPlacesCount() const { return _spawnPlacesCount; }

    void SetDeathFromEmptyEnergy(int64_t value) { _deathFromEmptyEnergy = value; }
    int64_t GetDeathFromEmptyEnergy() const { return _deathFromEmptyEnergy; }

    void SetDeathFromAge(int64_t value) { _deathFromAge = value; }
    int64_t GetDeathFromAge() const { return _deathFromAge; }

private:
    int64_t _cellsCount { 0 };
    int64_t _spawnPlacesCount { 0 };
    int64_t _cellsCapacity { 0 };
    int64_t _deathFromEmptyEnergy { 0 };
    int64_t _deathFromAge { 0 };
};