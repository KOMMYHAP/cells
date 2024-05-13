#pragma once

#include "component.h"
#include "component_handle.h"
#include "component_storage.h"

class ComponentRegistry {
public:
    ComponentRegistry(size_t storageSize);
    ComponentHandle Register(const Component& component);
    void Freeze();

    ComponentStorage& Modify(const ComponentHandle& handle);
    const ComponentStorage& Get(const ComponentHandle& handle) const;

    size_t GetStorageSize() const { return _storageSize; }

private:
    bool _wasFreeze { false };
    size_t _storageSize { 0 };
    std::map<ComponentHandle, ComponentStorage> _storages;
    ComponentHandle _nextHandle;
};