#pragma once

#include "components/cell_id.h"

class IdSystem;
class PositionSystem;
class TypeSystem;

class GraveyardSystem {
public:
    GraveyardSystem(uint32_t capacity, IdSystem& idSystem, TypeSystem& typeSystem, PositionSystem& positionSystem);
    void Bury(CellId id);

    void Cleanup();

private:
    std::vector<bool> _graveyardFlags;
    std::vector<CellId> _graveyard;
    IdSystem& _idSystem;
    TypeSystem& _typeSystem;
    PositionSystem& _positionSystem;
};