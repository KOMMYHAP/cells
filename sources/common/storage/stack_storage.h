#pragma once

#include "storage.h"

namespace common {

class StackStorage {
public:
    StackStorage() noexcept = default;
    StackStorage(const StackStorage& other) = delete;
    StackStorage(StackStorage&& other) noexcept = default;
    StackStorage& operator=(const StackStorage& other) = delete;
    StackStorage& operator=(StackStorage&& other) noexcept = default;
    ~StackStorage() noexcept;

public:
    template <class T>
    T& Modify() const;

    template <class T>
    const T& Get() const;

    template <class T, class... Args>
        requires std::constructible_from<T, Args...>
    T& Store(Args&&... args);

    template <class T>
    void Remove();

    template <class T>
    bool Has() const;

    size_t Count() const;

private:
    std::stack<Storage::ItemType> _order;
    Storage _storage;
};
}

#include "stack_storage.hpp"