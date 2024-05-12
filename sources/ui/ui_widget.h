#pragma once

class UiWidget {
public:
    virtual ~UiWidget() = default;
    virtual void Update(sf::Time elapsedTime) = 0;
    virtual void Draw(sf::RenderTarget& target) = 0;
};

struct UiHandle {
    uint32_t id { 0 };

    friend std::strong_ordering operator<=>(const UiHandle& lhs, const UiHandle& rhs) = default;
};