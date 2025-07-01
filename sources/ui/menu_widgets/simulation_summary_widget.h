#pragma once

#include "base/base_menu_widget.h"

class WorldStatistics;
class SimulationStatisticsProvider;

class SimulationSummaryWidget final : public BaseMenuWidget {
public:
    SimulationSummaryWidget(const WorldStatistics & worldStats, const SimulationStatisticsProvider& simulationStats);
    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    gsl::not_null<const WorldStatistics*> _worldStats;
    gsl::not_null<const SimulationStatisticsProvider*> _simulationStats;
};