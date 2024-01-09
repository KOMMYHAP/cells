#pragma once

#include "cell.h"
#include "field.h"

namespace Details {

template <class CellType>
class FieldIteratorBase {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = CellType;
    using pointer = value_type*;
    using reference = value_type&;

    bool Invalid() const;
    bool Valid() const;

    CellId GetCellId() const;

    reference operator*() const { return *_cell; }
    pointer operator->() { return _cell; }

    FieldIteratorBase& operator++();
    FieldIteratorBase operator++(int);

    FieldIteratorBase& operator--();
    FieldIteratorBase operator--(int);

    friend bool operator==(const FieldIteratorBase& a, const FieldIteratorBase& b) { return a._cell == b._cell; };
    friend bool operator!=(const FieldIteratorBase& a, const FieldIteratorBase& b) { return !(a == b); };

protected:
    explicit FieldIteratorBase(CellType* current, CellType* begin, CellType* end);

private:
    pointer _cell;
    pointer _firstCell;
    pointer _lastCell;
};
}

class FieldIterator : public Details::FieldIteratorBase<Cell> {
    using Base = Details::FieldIteratorBase<Cell>;

public:
    explicit FieldIterator(Field& field, bool begin);
};

class ConstFieldIterator : public Details::FieldIteratorBase<const Cell> {
    using Base = Details::FieldIteratorBase<const Cell>;

public:
    explicit ConstFieldIterator(const Field& field, bool begin);
};

FieldIterator begin(Field& field);
FieldIterator end(Field& field);
ConstFieldIterator begin(const Field& field);
ConstFieldIterator end(const Field& field);

#include "field_iterator.hpp"