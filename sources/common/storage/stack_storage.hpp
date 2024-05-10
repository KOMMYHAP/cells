#pragma once

namespace common {
inline size_t StackStorage::Count() const
{
    return _storage.Count();
}

template <class T>
bool StackStorage::Has() const
{
    return _storage.Has<T>();
}

template <class T>
void StackStorage::Remove()
{
    ASSERT(_order.top() == typeid(T), "Trying to remove item in wrong order!");
    _order.pop();
    return _storage.Remove<T>();
}

template <class T, class... Args>
    requires std::constructible_from<T, Args...>
T& StackStorage::Store(Args&&... args)
{
    _order.push(typeid(T));
    return _storage.Store<T>(std::forward<Args>(args)...);
}

template <class T>
const T& StackStorage::Get() const
{
    return _storage.Get<T>();
}

template <class T>
T& StackStorage::Modify() const
{
    return _storage.Modify<T>();
}

}