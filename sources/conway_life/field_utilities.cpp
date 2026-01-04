#include "field_utilities.h"

#include "field.h"

std::size_t FieldHasher::operator()(const Field& field) const
{
    return std::hash<std::vector<bool>> {}(field._cells);
}

bool FieldComparator::operator()(const Field& lhs, const Field& rhs) const
{
    return lhs._cells == rhs._cells;
}