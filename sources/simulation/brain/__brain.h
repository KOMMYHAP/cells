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
// class Brain : public Details::BrainBase<Brain> {
// public:
//     Brain(Brain& cell);
//
//     Memory AccessMemory();
// };
//
// class ConstBrain : public Details::BrainBase<const Brain> {
// public:
//     ConstBrain(const Brain& cell);
// };
//
// #include "brain.hpp"