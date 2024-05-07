#pragma once

class PositionSystem;
class BrainSystem;
class IdSystem;
class TypeSystem;
class HealthSystem;
class ICellFactory;
class CellBrain;
class Spawner;
enum class CellId : uint32_t;

namespace common {
class Storage;
}

class SpawnSystem {
public:
    struct Config {
        common::Storage& systems;
        uint32_t populationSize;
    };
    SpawnSystem(Config&& config);

    void TryToSpawn();
    void SetCellFactory(ICellFactory& factory);

private:
    void SpawnN(uint32_t cellsCount);
    std::optional<CellBrain> TryMakeCellBrain();

    PositionSystem* _positionSystem { nullptr };
    IdSystem* _idSystem { nullptr };
    ICellFactory* _cellFactory { nullptr };
    Spawner* _spawner { nullptr };
    uint32_t _targetPopulationSize { 0 };
};
