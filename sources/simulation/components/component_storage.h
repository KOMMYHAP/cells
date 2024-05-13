#pragma once

#include "component.h"

class ComponentStorage {
public:
    ComponentStorage() = default;
    ComponentStorage(const Component& metaInfo, size_t count);

    const Component& GetMetaInfo() const { return _info; };

    std::byte& ModifyUnsafe(size_t componentIndex);
    const std::byte& GetUnsafe(size_t componentIndex) const;

    template <class T>
    T& Modify(size_t componentIndex);

    template <class T>
    const T& Get(size_t componentIndex) const;

    size_t Count() const;
    size_t CountBytes() const;

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
