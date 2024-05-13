#include "system.h"
#include "components/component_registry.h"
#include "components/component_storage.h"

System::System(ComponentRegistry& registry, const std::vector<ComponentHandle>& handles)
    : _componentRegistry(registry)
{
    _componentInfoList.reserve(handles.size());
    _componentBuffer.reserve(handles.size());
    for (const ComponentHandle& handle : handles) {
        ComponentStorage& storage = _componentRegistry.Modify(handle);
        std::byte* firstComponent = &storage.ModifyUnsafe(0);
        _componentInfoList.emplace_back(handle, &storage.GetMetaInfo(), firstComponent);

        _componentBuffer.push_back(firstComponent);
    }
}

void System::Foreach(const std::function<void(const Context&)>& func)
{
    const size_t componentsCount = _componentInfoList.size();

    for (size_t i = 0; i < componentsCount; ++i) {
        const ComponentHandle handle = _componentInfoList[i].handle;
        ComponentStorage& storage = _componentRegistry.Modify(handle);
        std::byte* item = &storage.ModifyUnsafe(0);
        _componentBuffer[i] = item;
    }

    const size_t itemsCount = _componentRegistry.GetStorageSize();
    Context context { _componentBuffer };

    for (size_t itemIndex = 0; itemIndex < itemsCount; ++itemIndex) {
        func(context);

        for (size_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            std::byte*& componentAddress = _componentBuffer[componentIndex];
            componentAddress += _componentInfoList[componentIndex].meta->sizeInBytes;
        }
    }

    for (size_t i = 0; i < _componentInfoList.size(); ++i) {
        _componentBuffer[i] = _componentInfoList[i].startAddress;
    }
}
