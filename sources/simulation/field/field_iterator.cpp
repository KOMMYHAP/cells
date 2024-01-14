#include "field_iterator.h"

FieldIterator begin(Field& field)
{
    return FieldIterator(field, true);
}

FieldIterator end(Field& field)
{
    return FieldIterator(field, false);
}

ConstFieldIterator begin(const Field& field)
{
    return ConstFieldIterator(field, true);
}

ConstFieldIterator end(const Field& field)
{
    return ConstFieldIterator(field, false);
}

FieldIterator::FieldIterator(Field& field, bool begin)
    : Base(
        begin ? std::to_address(field._cells.begin()) : std::to_address(field._cells.end()),
        std::to_address(field._cells.begin()),
        std::to_address(field._cells.end()))
{
}

ConstFieldIterator::ConstFieldIterator(const Field& field, bool begin)
    : Base(
        begin ? std::to_address(field._cells.begin()) : std::to_address(field._cells.end()),
        std::to_address(field._cells.begin()),
        std::to_address(field._cells.end()))
{
}
