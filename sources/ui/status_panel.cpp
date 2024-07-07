#include "status_panel.h"
#include "world.h"

constexpr uint16_t StatusMessageBufferLimit = 200;
constexpr uint16_t StatusTextSize = 10;
constexpr uint16_t StatusTextOffset = 0;

static auto GetTimeInfo(sf::Time time)
{
    const std::string_view tickUnit = time.asMilliseconds() >= 1000
        ? "s"
        : time.asMicroseconds() >= 1000
        ? "ms"
        : "us";
    const float tickTimeValue = time.asMilliseconds() >= 1000
        ? time.asSeconds()
        : time.asMicroseconds() >= 1000
        ? static_cast<float>(time.asMilliseconds())
        : static_cast<float>(time.asMicroseconds());

    return std::make_tuple(tickTimeValue, tickUnit);
}

StatusPanel::StatusPanel(const UiLayout& layout, const sf::Font& font, World& world)
    : _world(world)
{
    _text.setFont(font);
    _text.setCharacterSize(StatusTextSize);
    _text.setPosition(NarrowCast<float>(layout.fieldOffset + StatusTextSize), StatusTextOffset);

    _buffer.reserve(StatusMessageBufferLimit);
}

void StatusPanel::Update(sf::Time elapsedTime)
{
    _frameTimeCounter.AddSample(elapsedTime.asSeconds());
    sf::Time frameTime = sf::seconds(_frameTimeCounter.CalcMedian());

    const auto [frameTimeValue, frameUnit] = GetTimeInfo(frameTime);
    const auto fps = static_cast<uint16_t>(1.0f / frameTime.asSeconds());

    // todo:
    // convert to native component?
    // extract debug system to lua?
    //
    //    auto& idSystem = _world.GetSystems().Get(_idSystem);
    //    const uint32_t cellsCount = idSystem.GetCellsCount();
    //    const float cellsCountPercent = static_cast<uint8_t>(static_cast<float>(idSystem.GetCellsCount()) / (idSystem.GetCellsCountLimit()) * 100.0f);
    //
    constexpr uint32_t cellsCount = 0;
    constexpr float cellsCountPercent = 0.0f;

    _buffer.clear();
    std::format_to_n(std::back_inserter(_buffer), _buffer.capacity(),
        "FPS {:4} | Frame {:4}{:2} | Cells {:8} ({:2}%)",
        fps,
        frameTimeValue, frameUnit,
        cellsCount, cellsCountPercent);
    _text.setString(sf::String(_buffer));
}

void StatusPanel::Draw(sf::RenderTarget& target)
{
    target.draw(_text);
}