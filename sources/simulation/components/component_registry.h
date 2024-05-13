#pragma once

#include "component.h"
#include "component_handle.h"
#include "component_storage.h"

class ComponentRegistry {
public:
    ComponentRegistry(uint32_t cellsCount);
    ComponentHandle Register(const Component& component);
    void Freeze();

    ComponentStorage& Modify(const ComponentHandle& handle);
    const ComponentStorage& Get(const ComponentHandle& handle) const;

    uint32_t GetCellsCount() const { return _cellsCount; }

private:
    bool _wasFreeze { false };
    uint32_t _cellsCount { 0 };
    std::map<ComponentHandle, ComponentStorage> _storages;
    ComponentHandle _nextHandle;
};