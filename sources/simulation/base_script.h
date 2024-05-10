#pragma once

class BaseScript {
public:
    virtual ~BaseScript() = default;
    virtual std::error_code Perform() = 0;
};