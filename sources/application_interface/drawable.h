#pragma once

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void Draw() = 0;
};