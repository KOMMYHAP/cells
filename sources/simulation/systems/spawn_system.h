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

class SpawnSystem {
public:
    struct Config {
        PositionSystem& positionSystem;
        IdSystem& idSystem;
        Spawner& spawner;
        uint32_t populationSize;
    };
    SpawnSystem(Config&& config);

    void TryToSpawn();
    void SetCellFactory(ICellFactory& factory);

private:
    void SpawnN(uint32_t cellsCount);
    std::optional<CellBrain> TryMakeCellBrain();

    PositionSystem& _positionSystem;
    IdSystem& _idSystem;
    ICellFactory* _cellFactory { nullptr };
    Spawner& _spawner;
    uint32_t _targetPopulationSize { 0 };
};
