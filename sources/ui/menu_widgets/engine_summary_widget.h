#pragma once

#include "base/base_menu_widget.h"
#include "sample_counter.h"

class AppStatistics;
class EngineSummaryWidget final : public BaseMenuWidget {
public:
    EngineSummaryWidget(const AppStatistics& appStats);
    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    gsl::not_null<const AppStatistics*> _appStats;
};