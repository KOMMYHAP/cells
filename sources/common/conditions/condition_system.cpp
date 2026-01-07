#include "conditions/condition_system.h"
namespace Common {

Condition ConditionSystem::Register(std::string name, CheckFunction callback)
{
    const Condition condition { static_cast<uint16_t>(_conditions.size()) };
    _conditions.emplace_back(std::move(name), std::move(callback));
    _enabled.push_back(false);
    return condition;
}

void ConditionSystem::UpdateConditions()
{
    for (const auto& [index, data] : std::views::enumerate(_conditions)) {
        _enabled[index] = data.callback();
    }
}

bool ConditionSystem::CheckAllOf(std::span<Condition> conditions) const
{
    bool result = true;
    for (const Condition condition : conditions) {
        const auto index = static_cast<std::size_t>(condition);
        result &= _enabled[index];
    }
    return result;
}

bool ConditionSystem::Check(Condition condition) const
{
    return CheckAllOf(std::span { &condition, 1 });
}

}