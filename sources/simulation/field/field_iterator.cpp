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
        begin ? field._cells.begin().base() : field._cells.end().base(),
        field._cells.begin().base(),
        field._cells.end().base())
{
}

ConstFieldIterator::ConstFieldIterator(const Field& field, bool begin)
    : Base(
        begin ? field._cells.begin().base() : field._cells.end().base(),
        field._cells.begin().base(),
        field._cells.end().base())
{
}
