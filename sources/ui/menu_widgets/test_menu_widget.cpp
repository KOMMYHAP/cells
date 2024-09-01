#include "test_menu_widget.h"

void TestMenuWidget::OnMenuItemOpenedFirstTime()
{
    std::cout << "OnMenuItemOpenedFirstTime\n";
}

void TestMenuWidget::OnMenuItemJustOpened()
{
    std::cout << "OnMenuItemJustOpened\n";
}

BaseMenuWidget::MenuWidgetAction TestMenuWidget::ProcessMenuItem(sf::Time elapsedTime)
{
    _elapsedTimeSinceFlush += elapsedTime;
    static const sf::Time FlushPeriod { sf::seconds(1) };
    if (_elapsedTimeSinceFlush > FlushPeriod) {
        std::cout << "ProcessMenuItem\n";
        Flush();
    }
    ImGui::Text("Elapsed time since last flush: %4d [ms]", _elapsedTimeSinceFlush.asMilliseconds());
    return MenuWidgetAction::KeepOpen;
}

void TestMenuWidget::OnMenuItemJustClosed()
{
    std::cout << "OnMenuItemJustClosed\n";
    // Force flush on close, because there will not be any processing for this widget.
    Flush();
}

void TestMenuWidget::Flush()
{
    _elapsedTimeSinceFlush = {};
    std::cout.flush();
}