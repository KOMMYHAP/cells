#pragma once

class UiApplicationInterface {
public:
    virtual ~UiApplicationInterface() = default;
    virtual void ApplicationRunMainLoop() = 0;
};