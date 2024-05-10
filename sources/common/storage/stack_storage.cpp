#include "stack_storage.h"
namespace common {

StackStorage::~StackStorage() noexcept
{
    while (!_order.empty()) {
        const auto itemType = _order.top();
        _order.pop();
        _storage.Remove(itemType);
    }
}

}
