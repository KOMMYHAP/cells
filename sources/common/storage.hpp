#pragma once

namespace common {

template <class T>
T& Storage::Modify() const
{
    Item& item = Modify(typeid(T));
    T* typedItem = boost::anys::any_cast<T>(&item);
    ASSERT(typedItem);
    return *typedItem;
}

template <class T>
const T& Storage::Get() const
{
    const Item& item = Get(typeid(T));
    const T* typedItem = boost::anys::any_cast<T>(&item);
    ASSERT(typedItem);
    return *typedItem;
}

template <class T, class... Args>
    requires std::constructible_from<T, Args...>
T& Storage::Store(Args&&... args)
{
    auto item = Item(boost::anys::in_place_type_t<T>{}, std::forward<Args>(args)...);
    Item& storedItem = Store(std::move(item));
    T* typedItem = boost::anys::any_cast<T>(&storedItem);
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