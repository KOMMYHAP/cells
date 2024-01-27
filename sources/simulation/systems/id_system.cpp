#include "id_system.h"

IdSystem::IdSystem(uint32_t capacity)
    : _capacity(capacity)
    , _validCells(capacity, false)
{
    _freeIds.resize(capacity);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }
}

CellId IdSystem::Create()
{
    CellId id;
    if (!_freeIds.empty()) {
        id = _freeIds.back();
        _freeIds.pop_back();
    } else {
        id = MakeNextId();
        if (id == CellId::Invalid) {
            return CellId::Invalid;
        }
    }

    const auto index = CellIdToInt(id);
    _validCells[index] = true;
    return id;
}

void IdSystem::Remove(CellId id)
{
    const auto index = CellIdToInt(id);
    assert(index < _validCells.size());
    _validCells[index] = false;
    _freeIds.push_back(id);
}

CellId IdSystem::MakeNextId()
{
    if (_nextId == CellId::Invalid) {
        assert(false);
        return _nextId;
    }

    CellId id = _nextId;
    _nextId = static_cast<CellId>(CellIdToInt(_nextId) + 1);
    return id;
}
