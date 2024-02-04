#pragma once

class PositionSystem;
class BrainSystem;
class IdSystem;
class TypeSystem;
class ICellFactory;
enum class CellId : uint32_t;

class SpawnSystem {
public:
    struct Config {
        PositionSystem& positionSystem;
        IdSystem& idSystem;
        TypeSystem& typeSystem;
        BrainSystem& brainSystem;
        uint32_t populationSize;
    };
    SpawnSystem(Config&& config);

    void TryToSpawn();
    void SetCellFactory(ICellFactory& factory);

private:
    void SpawnN(uint32_t cellsCount);
    bool SpawnUnit(CellId id);

    void DebugDumpAliveCells();

    PositionSystem& _positionSystem;
    IdSystem& _idSystem;
    TypeSystem& _typeSystem;
    BrainSystem& _brainSystem;
    ICellFactory* _cellFactory { nullptr };
    uint32_t _targetPopulationSize { 0 };
};
