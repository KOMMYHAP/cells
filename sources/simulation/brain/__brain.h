// #pragma once
//
// #include "brain_packed_data.h"
// #include "cell.h"
// #include "processor/memory.h"
//
// namespace Details {
//
// template <class Unit>
// class BrainBase {
// public:
//     BrainBase(Unit& cell);
//
//     ConstMemory GetMemory() const;
//
// protected:
//     Unit& _cell;
// };
//
// }
//
// class CellBrain : public Details::BrainBase<CellBrain> {
// public:
//     CellBrain(CellBrain& cell);
//
//     Memory AccessMemory();
// };
//
// class ConstBrain : public Details::BrainBase<const CellBrain> {
// public:
//     ConstBrain(const CellBrain& cell);
// };
//
// #include "brain.hpp"