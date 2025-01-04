#include "test_menu_widget.h"

void TestMenuWidget::OnMenuItemOpenedFirstTime()
{
    std::cout << "OnMenuItemOpenedFirstTime\n";
}

void TestMenuWidget::OnMenuItemJustOpened()
{
    std::cout << "OnMenuItemJustOpened\n";
}

BaseMenuWidget::MenuWidgetAction TestMenuWidget::ProcessMenuItem(Common::Time elapsedTime)
{
    _elapsedTimeSinceFlush += elapsedTime;
    static const Common::Time FlushPeriod { Common::Time::FromSeconds(1) };
    if (_elapsedTimeSinceFlush > FlushPeriod) {
        Flush();
    }
    ImGui::Text("Elapsed time since last flush: %4d [ms]", static_cast<int>(_elapsedTimeSinceFlush.AsMilliseconds()));
    return MenuWidgetAction::KeepOpen;
}

void TestMenuWidget::OnMenuItemJustClosed()
{
    // Force flush on close, because there will not be any processing for this widget.
    Flush();
}

void TestMenuWidget::Flush()
{
    _elapsedTimeSinceFlush = {};
    std::cout.flush();
}