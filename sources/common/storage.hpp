#pragma once

namespace common {

template <class T>
T& Storage::Modify()
{
    std::any& item = Modify(typeid(T));
    T* typedItem = std::any_cast<T>(&item);
    ASSERT(typedItem);
    return *typedItem;
}

template <class T>
const T& Storage::Get() const
{
    const std::any& item = Get(typeid(T));
    const T* typedItem = std::any_cast<T>(&item);
    ASSERT(typedItem);
    return *typedItem;
}

template <class T, class... Args>
T& Storage::Store(Args&&... args)
{
    std::any item = std::make_any<T>(std::forward<Args>(args)...);
    std::any& storedItem = Store(std::move(item));
    T* typedItem = std::any_cast<T>(&storedItem);
    ASSERT(typedItem);
    return *typedItem;
}

template <class T>
void Storage::Remove()
{
    Remove(typeid(T));
}

template <class T>
bool Storage::Has() const
{
    return Has(typeid(T));
}

}