#include "graveyard_system.h"
#include "id_system.h"
#include "position_system.h"
#include "type_system.h"

GraveyardSystem::GraveyardSystem(uint32_t capacity, IdSystem& idSystem, TypeSystem& typeSystem, PositionSystem& positionSystem)
    : _graveyardFlags(capacity, false)
    , _idSystem(idSystem)
    , _typeSystem(typeSystem)
    , _positionSystem(positionSystem)
{
    _graveyard.reserve(capacity);
}

void GraveyardSystem::Bury(CellId id)
{
    const auto index = CellIdToInt(id);
    ASSERT(index < _graveyardFlags.size());
    if (_graveyardFlags[index]) {
        return;
    }

    _graveyardFlags[index] = true;
    ASSERT(_graveyard.size() + 1 < _graveyard.capacity());
    _graveyard.push_back(id);
}

void GraveyardSystem::Cleanup()
{
    std::sort(_graveyard.begin(), _graveyard.end());

    for (const CellId id : _graveyard) {
        // cleanup id to allow birth of a new cell
        _idSystem.Remove(id);

        // cleanup type to exclude cell from simulation
        _typeSystem.Set(id, CellType::Dummy);

        // cleanup position to allow everyone move to owner's position
        _positionSystem.Set(id, InvalidCellPosition);

        // we can omit cleanup brain, because it required only in simulation
        // we can omit cleanup health, because we are here only when health is zero
    }

    _graveyard.clear();
    std::fill(_graveyardFlags.begin(), _graveyardFlags.end(), false);
}