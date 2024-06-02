#include "type_system.h"

TypeSystem::TypeSystem(uint32_t capacity)
    : _types(capacity, CellType::Dummy) {}

void TypeSystem::Set(CellId id, CellType type)
{
    const auto index = CellIdToInt(id);
    Expects(index <= _types.size());
    _types[index] = type;
}

CellType TypeSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    Expects(index <= _types.size());
    return _types[index];
}