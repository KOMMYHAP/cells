#pragma once

template <class T>
    requires std::is_trivially_default_constructible_v<T>
void ComponentStorage::VerifyComponent() const
{
    constexpr size_t requestedSize = sizeof(T);
    ASSERT(_info.sizeInBytes == requestedSize, "Invalid component size", requestedSize, _info.sizeInBytes);
}

template <class T>
const T& ComponentStorage::Get(CellId id) const
{
    VerifyComponent<T>();
    const std::byte& address = GetUnsafe(id);
    return reinterpret_cast<const T&>(address);
}

template <class T>
T& ComponentStorage::Modify(CellId id)
{
    VerifyComponent<T>();
    std::byte& address = ModifyUnsafe(id);
    return reinterpret_cast<T&>(address);
}

template <class T, class Func>
    requires std::invocable<Func, T&>
void ComponentStorage::Foreach(Func&& func)
{
    if (GetCellsCount() == 0) {
        return;
    }
    T* item = &Modify<T>(CellId { 0 });
    const size_t count = GetCellsCount();
    for (size_t i = 0; i < count; ++i) {
        func(*item);
        ++item;
    }
}

template <class T, class Func>
    requires std::invocable<Func, const T>
void ComponentStorage::Foreach(Func&& func) const
{
    if (GetCellsCount() == 0) {
        return;
    }
    const T* item = &Get<T>(CellId { 0 });
    const size_t count = GetCellsCount();
    for (size_t i = 0; i < count; ++i) {
        func(*item);
        ++item;
    }
}