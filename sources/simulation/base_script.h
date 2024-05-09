#pragma once

class BaseScript {
public:
    virtual ~BaseScript() = default;
    virtual std::expected<void, std::error_code> Perform() = 0;
};