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