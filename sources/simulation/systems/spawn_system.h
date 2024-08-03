#pragma once

class PositionSystem;
class BrainSystem;
class IdSystem;
class TypeSystem;
class HealthSystem;
class ICellFactory;
struct CellBrain;
class Spawner;
enum class CellId : uint32_t;

namespace common {
class Storage;
}

class SpawnSystem {
public:
    SpawnSystem(PositionSystem& positionSystem, IdSystem& idSystem, Spawner& spawner);
    void SetSpawnLimit(uint32_t populationSize);

    void TryToSpawn();
    void SetCellFactory(ICellFactory& factory);

private:
    void SpawnN(uint32_t cellsCount);
    std::optional<CellBrain> TryMakeCellBrain();

    PositionSystem& _positionSystem;
    IdSystem& _idSystem;
    Spawner& _spawner;
    ICellFactory* _cellFactory { nullptr };
    uint32_t _targetPopulationSize { 0 };
};