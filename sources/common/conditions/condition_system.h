#pragma once
#include "condition.h"

namespace Common {

class ConditionSystem {
public:
    using CheckFunction = std::move_only_function<bool()>;
    Condition Register(std::string name, CheckFunction callback);

    void UpdateConditions();

    bool CheckAllOf(std::span<Condition> conditions) const;
    bool Check(Condition condition) const;

private:
    struct ConditionData {
        std::string name;
        CheckFunction callback;
    };

    std::vector<bool> _enabled;
    std::vector<ConditionData> _conditions;
};

}