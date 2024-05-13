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
    ResetComponentBuffer();

    const size_t componentsCount = _componentInfoList.size();
    const size_t itemsCount = _componentRegistry.GetStorageSize();
    const Context context { _componentBuffer };

    for (size_t itemIndex = 0; itemIndex < itemsCount; ++itemIndex) {
        func(context);

        for (size_t componentIndex = 0; componentIndex < componentsCount; ++componentIndex) {
            std::byte*& componentAddress = _componentBuffer[componentIndex];
            componentAddress += _componentInfoList[componentIndex].meta->sizeInBytes;
        }
    }
}

void System::InitComponents(const std::vector<ComponentHandle>& handles)
{
    ASSERT(_componentInfoList.empty() && _componentBuffer.empty(), "Component was already initialized!");
    _componentInfoList.reserve(handles.size());
    _componentBuffer.reserve(handles.size());
    for (const ComponentHandle& handle : handles) {
        ComponentStorage& storage = _componentRegistry.Modify(handle);
        std::byte* firstComponent = &storage.ModifyUnsafe(0);
        _componentInfoList.emplace_back(handle, &storage.GetMetaInfo(), firstComponent);

        _componentBuffer.push_back(firstComponent);
    }
}

void System::ResetComponentBuffer()
{
    for (size_t i = 0; i < _componentInfoList.size(); ++i) {
        _componentBuffer[i] = _componentInfoList[i].startAddress;
    }
}
