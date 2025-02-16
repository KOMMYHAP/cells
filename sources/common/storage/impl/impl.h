#pragma once

namespace common::impl {

class StorageItemHolderBase {
public:
    virtual ~StorageItemHolderBase() = default;
    virtual std::type_index GetTypeIndex() = 0;
};

template <class T>
class StorageItemHolder final : public StorageItemHolderBase {
public:
    template <class... Args>
    explicit StorageItemHolder(Args&&... args)
        : value(std::forward<Args>(args)...)
    {
    }

    std::type_index GetTypeIndex() override { return typeid(T); }

    T value;
};

}