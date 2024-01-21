#include "id_system.h"

IdSystem::IdSystem(uint32_t capacity)
    : _capacity(capacity)
{
    _freeIds.resize(capacity);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }
}

CellId IdSystem::Create()
{
    if (!_freeIds.empty()) {
        CellId id = _freeIds.back();
        _freeIds.pop_back();
        return id;
    }

    return MakeNextId();
}

void IdSystem::Remove(CellId id)
{
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
