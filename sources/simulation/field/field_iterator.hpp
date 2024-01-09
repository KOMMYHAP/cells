#pragma once
#include <utility>

#include "brain/brain.h"
#include "brain/brain_packed_data.h"

namespace Details {

template <class CellType>
FieldIteratorBase<CellType>::FieldIteratorBase(CellType* current, CellType* begin, CellType* end)
    : _cell(current)
    , _firstCell(begin)
    , _lastCell(end)
{
}

template <class Cell>
FieldIteratorBase<Cell>& FieldIteratorBase<Cell>::operator++()
{
    do {
        ++_cell;
    } while (_cell < _lastCell && Invalid());
    return *this;
}

template <class Cell>
FieldIteratorBase<Cell> FieldIteratorBase<Cell>::operator++(int)
{
    FieldIteratorBase<Cell> tmp = *this;
    ++(*this);
    return tmp;
}

template <class Cell>
FieldIteratorBase<Cell>& FieldIteratorBase<Cell>::operator--()
{
    do {
        --_cell;
    } while (_cell > _firstCell && Invalid());
    return *this;
}

template <class Cell>
FieldIteratorBase<Cell> FieldIteratorBase<Cell>::operator--(int)
{
    FieldIteratorBase<Cell> tmp = *this;
    --(*this);
    return tmp;
}

template <class Cell>
bool FieldIteratorBase<Cell>::Invalid() const
{
    assert(_cell != _lastCell);
    return ConstBrain(*_cell).GetInfo().type == CellType::Dummy;
}

template <class Cell>
bool FieldIteratorBase<Cell>::Valid() const
{
    return !Invalid();
}

template <class CellType>
CellId FieldIteratorBase<CellType>::GetCellId() const
{
    const auto diff = _cell - _firstCell;
    return static_cast<CellId>(diff);
}

}