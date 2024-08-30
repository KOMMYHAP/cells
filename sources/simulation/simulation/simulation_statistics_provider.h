#pragma once

class CellLocator;

class SimulationStatisticsProvider {
public:
    explicit SimulationStatisticsProvider(const CellLocator& locator);

    size_t GetCellsCapacity() const { return _cellsCapacity; }

    void SetCellsCount(size_t cellsCount) { _cellsCount = cellsCount; }
    size_t GetCellsCount() const { return _cellsCount; }

    void SetSpawnPlacesCount(size_t spawnPlacesCount) { _spawnPlacesCount = spawnPlacesCount; }
    size_t GetSpawnPlacesCount() const { return _spawnPlacesCount; }

    void SetDeathFromEmptyEnergy(size_t value) { _deathFromEmptyEnergy = value; }
    size_t GetDeathFromEmptyEnergy() const { return _deathFromEmptyEnergy; }

    void SetDeathFromAge(size_t value) { _deathFromAge = value; }
    size_t GetDeathFromAge() const { return _deathFromAge; }

private:
    size_t _cellsCount { 0 };
    size_t _spawnPlacesCount { 0 };
    size_t _cellsCapacity { 0 };
    size_t _deathFromEmptyEnergy { 0 };
    size_t _deathFromAge { 0 };
};