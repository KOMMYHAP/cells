#pragma once

namespace common {

class Storage {
public:
    Storage();
    ~Storage();

    template <class T>
    T& Modify();

    template <class T>
    const T& Get() const;

    template <class T, class... Args>
    T& Store(Args&&... args);

    template <class T>
    void Remove();

    template <class T>
    bool Has() const;

    size_t Count() const;

public:
    std::any& Modify(std::type_index index);
    const std::any& Get(std::type_index index) const;

    std::any& Store(std::any item);
    void Remove(std::type_index index);

    bool Has(std::type_index index) const;

private:
    std::map<std::type_index, std::any> _items;
};
}

#include "storage.hpp"