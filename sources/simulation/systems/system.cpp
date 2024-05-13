#include "system.h"
#include "components/component_registry.h"
#include "components/component_storage.h"

System::System(ComponentRegistry& registry, const std::vector<ComponentHandle>& handles)
    : _componentRegistry(registry)
{
    InitComponents(handles);
}

void System::Foreach(const std::function<void(const Context&)>& func)
{
    // Reset component buffer to iterate items from start.
    for (size_t i = 0; i < _componentInfoList.size(); ++i) {
        _componentBuffer[i] = _componentInfoList[i].startAddress;
    }

    const uint32_t componentsCount = _componentInfoList.size();
    const uint32_t itemsCount = _componentRegistry.GetCellsCount();

    for (uint32_t itemIndex = 0; itemIndex < itemsCount; ++itemIndex) {
        const Context context { CellId { itemIndex }, _componentBuffer };
        func(context);

        for (uint32_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            std::byte*& componentAddress = _componentBuffer[componentIndex];
            componentAddress += _componentInfoList[componentIndex].sizeInBytes;
        }
    }
}

void System::Message(const std::span<CellId>& cells, const std::function<void(const Context&)>& func)
{
    const size_t componentsCount = _componentInfoList.size();
    const size_t itemsCount = _componentRegistry.GetCellsCount();

    for (const CellId cellId : cells) {
        const auto itemIndex = static_cast<uint32_t>(cellId);
        ASSUME(itemIndex < itemsCount);

        for (size_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            const ComponentInfo& info = _componentInfoList[componentIndex];
            const uint32_t itemOffset = info.sizeInBytes * itemIndex;
            _componentBuffer[componentIndex] = info.startAddress + itemOffset;
        }

        const Context context { cellId, _componentBuffer };
        func(context);
    }
}

void System::InitComponents(const std::vector<ComponentHandle>& handles)
{
    ASSERT(_componentInfoList.empty() && _componentBuffer.empty(), "Component was already initialized!");
    _componentInfoList.reserve(handles.size());
    _componentBuffer.reserve(handles.size());
    for (const ComponentHandle& handle : handles) {
        ComponentStorage& storage = _componentRegistry.Modify(handle);
        std::byte* firstComponent = &storage.ModifyUnsafe(CellId { 0 });
        _componentInfoList.emplace_back(handle, storage.GetMetaInfo().sizeInBytes, firstComponent);

        _componentBuffer.push_back(firstComponent);
    }
}
