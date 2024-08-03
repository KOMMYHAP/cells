#pragma once

#include "component.h"
#include "components/cell_id.h"

class ComponentStorage {
public:
    ComponentStorage() = default;
    ComponentStorage(const Component& metaInfo, uint32_t cellsCount);

    const Component& GetMetaInfo() const { return _info; };

    std::byte& ModifyUnsafe(CellId id);
    const std::byte& GetUnsafe(CellId id) const;

    template <class T>
    T& Modify(CellId id);

    template <class T>
    const T& Get(CellId id) const;

    uint32_t GetCellsCount() const;

    template <class T, class Func>
        requires std::invocable<Func, T&>
    void Foreach(Func&& func);

    template <class T, class Func>
        requires std::invocable<Func, const T>
    void Foreach(Func&& func) const;

private:
    template <class T>
        requires std::is_trivially_default_constructible_v<T>
    void VerifyComponent() const;

    Component _info;
    std::vector<std::byte> _buffer;
};

#include "component_storage.hpp"
