#pragma once

#include "base/base_menu_widget.h"

class SimulationPlayer;
class SimulationPlayerWidget final : public BaseMenuWidget {
public:
    SimulationPlayerWidget(SimulationPlayer& player);

    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    SimulationPlayer* _player{nullptr};
    static constexpr int32_t GenerationsPerSecondLimit{1000};
    int32_t _generationsPerSecond{0};
};