#include "field_utilities.h"

#include "field.h"

std::size_t FieldHasher::operator()(FieldHistory::Index field) const
{
    return operator()(history->_fields[std::to_underlying(field)]);
}

std::size_t FieldHasher::operator()(const Field& field) const
{
    return std::hash<std::vector<bool>> {}(field._cells);
}

bool FieldComparator::operator()(FieldHistory::Index lhsIndex, FieldHistory::Index rhsIndex) const
{
    const Field& lhs = history->_fields[std::to_underlying(lhsIndex)];
    const Field& rhs = history->_fields[std::to_underlying(rhsIndex)];
    return operator()(lhs, rhs);
}

bool FieldComparator::operator()(const Field& lhs, const Field& rhs) const
{
    return lhs._cells == rhs._cells;
}