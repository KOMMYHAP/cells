#include "simulation_summary_widget.h"

#include "simulation/simulation_statistics_provider.h"
#include "world.h"
#include "world_statistics.h"

SimulationSummaryWidget::SimulationSummaryWidget(World& world, const WorldStatistics& worldStats, const SimulationStatisticsProvider& stats)
    : _world(&world)
    , _worldStats(&worldStats)
    , _simulationStats(&stats)
{
}

BaseMenuWidget::MenuWidgetAction SimulationSummaryWidget::ProcessMenuItem(Common::Time /*time*/)
{
    ImGui::Text("Update: %" PRId64 " (%" PRId64 " ms)", _worldStats->GetElapsedFramesCount(), _worldStats->GetFrameTime().AsMilliseconds());
    ImGui::Text("Tick: %" PRId64 " (%" PRId64 " ms)", _worldStats->GetElapsedTicksCount(), _worldStats->GetTickTime().AsMilliseconds());
    ImGui::Text("Cells: %" PRId64 "/%" PRId64 " (alive/limit)", _simulationStats->GetCellsCount(), _simulationStats->GetCellsCapacity());
    ImGui::Text("Spawn places: %" PRId64, _simulationStats->GetSpawnPlacesCount());
    ImGui::Text("Death: energy %" PRId64 ", age %" PRId64, _simulationStats->GetDeathFromEmptyEnergy(), _simulationStats->GetDeathFromAge());
    return MenuWidgetAction::KeepOpen;
}