#include "system_base.h"

SystemBase::SystemBase(ComponentRegistry& registry, std::string_view name, const std::span<ComponentHandle>& handles)
    : _name(name)
{
    ASSERT(_componentInfoList.empty() && _componentBuffer.empty(), "Component was already initialized!");
    _componentInfoList.reserve(handles.size());
    _componentBuffer.reserve(handles.size());
    for (const ComponentHandle& handle : handles) {
        ComponentStorage& storage = registry.Modify(handle);
        std::byte* firstComponent = &storage.ModifyUnsafe(CellId{ 0 });
        _componentInfoList.emplace_back(handle, storage.GetMetaInfo().sizeInBytes, firstComponent);

        _componentBuffer.push_back(firstComponent);
    }
}

void SystemBase::ProcessImpl(std::span<const CellId> cells, const std::function<void(const SystemContext&)>& func)
{
    const size_t componentsCount = _componentInfoList.size();

    for (const CellId cellId : cells) {
        const auto itemIndex = static_cast<uint32_t>(cellId);
        ASSUME(itemIndex < _cellsCount);

        for (size_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            const ComponentInfo& info = _componentInfoList[componentIndex];
            const uint32_t itemOffset = info.sizeInBytes * itemIndex;
            _componentBuffer[componentIndex] = info.startAddress + itemOffset;
        }

        const SystemContext context{ cellId, _componentBuffer };
        func(context);
    }
}