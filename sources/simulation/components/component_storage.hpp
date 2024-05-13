#pragma once

template <class T>
    requires std::is_trivially_default_constructible_v<T>
void ComponentStorage::VerifyComponent() const
{
    constexpr size_t requestedSize = sizeof(T);
    ASSUME(_info.sizeInBytes == requestedSize, "Invalid component size", requestedSize, _info.sizeInBytes);
}

template <class T>
const T& ComponentStorage::Get(size_t componentIndex) const
{
    VerifyComponent<T>();
    const std::byte& address = GetUnsafe(componentIndex);
    return reinterpret_cast<const T&>(address);
}

template <class T>
T& ComponentStorage::Modify(size_t componentIndex)
{
    VerifyComponent<T>();
    std::byte& address = ModifyUnsafe(componentIndex);
    return reinterpret_cast<T&>(address);
}

template <class T, class Func>
    requires std::invocable<Func, T&>
void ComponentStorage::Foreach(Func&& func)
{
    if (Count() == 0) {
        return;
    }
    T* item = &Modify<T>(0);
    const size_t count = Count();
    for (size_t i = 0; i < count; ++i) {
        func(*item);
        ++item;
    }
}

template <class T, class Func>
    requires std::invocable<Func, const T>
void ComponentStorage::Foreach(Func&& func) const
{
    if (Count() == 0) {
        return;
    }
    const T* item = &Get<T>(0);
    const size_t count = Count();
    for (size_t i = 0; i < count; ++i) {
        func(*item);
        ++item;
    }
}