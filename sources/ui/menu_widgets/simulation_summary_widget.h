#pragma once

#include "base/base_menu_widget.h"

class World;
class WorldStatistics;
class SimulationStatisticsProvider;

class SimulationSummaryWidget final : public BaseMenuWidget {
public:
    SimulationSummaryWidget(World & world, const WorldStatistics & worldStats, const SimulationStatisticsProvider& simulationStats);
    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    gsl::not_null<World*> _world;
    gsl::not_null<const WorldStatistics*> _worldStats;
    gsl::not_null<const SimulationStatisticsProvider*> _simulationStats;
};