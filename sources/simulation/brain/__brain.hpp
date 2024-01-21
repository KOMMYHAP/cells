//#pragma once
//
//namespace Details {
//
//template <class Unit>
//BrainBase<Unit>::BrainBase(Unit& cell)
//    : _cell(cell)
//{
//}
//
//template <class Unit>
//ConstMemory BrainBase<Unit>::GetMemory() const
//{
//    const auto memory = std::span { std::as_const(_cell).data, std::as_const(_cell).data + Brain::Size };
//    return ConstMemory { memory };
//}
//
//}