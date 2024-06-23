#include "system_base.h"

SystemBase::SystemBase(ComponentRegistry& registry, std::span<const ComponentHandle> handles)
    : _cellsCount(registry.GetCellsCount())
{
    _components.reserve(handles.size());
    _componentBuffer.reserve(handles.size());
    for (const ComponentHandle& handle : handles) {
        ComponentStorage& storage = registry.Modify(handle);
        std::byte* firstComponent = &storage.ModifyUnsafe(CellId { 0 });
        _components.emplace_back(handle, storage.GetMetaInfo().sizeInBytes, firstComponent);

        _componentBuffer.push_back(firstComponent);
    }
}

void SystemBase::ExecutorProcess(std::span<const CellId> cells)
{
    const size_t componentsCount = _components.size();

    for (const CellId cellId : cells) {
        const auto itemIndex = static_cast<uint32_t>(cellId);
        ASSERT(itemIndex < _cellsCount);

        for (size_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            ComponentEntry& info = _components[componentIndex];
            std::byte& rawComponent = info.storage->ModifyUnsafe(cellId);
            _componentBuffer[componentIndex] = &rawComponent;
        }

        const SystemContext context { cellId, _componentBuffer };
        DoProcess(context);
    }
}