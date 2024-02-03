#pragma once

class PositionSystem;
class CellFactory;
class IdSystem;
class TypeSystem;
enum class CellId : uint32_t;

class SpawnSystem {
public:
    enum class Policy {
        PatrolUnit,
        RandomUnit
    };
    struct Config {
        CellFactory& factory;
        PositionSystem& positionSystem;
        IdSystem& idSystem;
        TypeSystem& typeSystem;
        uint32_t targetPopulationSize;
        Policy policy;
    };
    SpawnSystem(Config&& config);

    void Tick();

private:
    void SpawnN(uint32_t cellsCount);
    bool SpawnUnit(CellId id);

    CellFactory& _factory;
    PositionSystem& _positionSystem;
    IdSystem& _idSystem;
    TypeSystem& _typeSystem;
    uint32_t _targetPopulationSize { 0 };
    Policy _spawnPolicy { Policy::RandomUnit };
};
