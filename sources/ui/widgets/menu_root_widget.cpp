#include "menu_root_widget.h"

void MenuRootWidget::UpdateWidget(sf::Time elapsedTime)
{
    if (ImGui::BeginMainMenuBar()) {
        UpdateWidgetsGroup(_rootEntry, elapsedTime);
        ImGui::EndMainMenuBar();
    }
}

MenuWidgetId MenuRootWidget::AddWidget(MenuWidgetId parent, WidgetData widgetData)
{
    const auto childId = static_cast<MenuWidgetId>(_widgets.size());
    _widgets.emplace_back(std::move(widgetData));
    _indexedGroups[parent].items.emplace_back(childId);
    return childId;
}

void MenuRootWidget::UpdateWidgetsGroup(MenuWidgetId id, sf::Time elapsedTime)
{
    const auto groupIt = _indexedGroups.find(id);
    if (groupIt == _indexedGroups.end()) {
        return;
    }
    const WidgetsGroup& group = groupIt->second;
    if (group.items.empty()) {
        return;
    }

    for (const MenuWidgetId childId : group.items) {
        const WidgetNextGroup nextGroup = UpdateWidget(childId, elapsedTime);
        if (nextGroup.has_value()) {
            const MenuWidgetId childGroup = *nextGroup;
            UpdateWidgetsGroup(childGroup, elapsedTime);
        }
    }
}

MenuRootWidget::WidgetNextGroup MenuRootWidget::UpdateWidget(MenuWidgetId id, sf::Time elapsedTime)
{
    const auto index = static_cast<uint32_t>(id);
    ASSERT(index < _widgets.size());
    WidgetData& widgetData = _widgets[index];

    WidgetNextGroup nextGroup;
    if (ImGui::BeginMenu(widgetData.name.c_str())) {
        ProcessWidgetAction(widgetData, elapsedTime);
        if (widgetData.state.wasOpened) {
            nextGroup = id;
        }
        ImGui::EndMenu();
    }
    return nextGroup;
}

void MenuRootWidget::ProcessWidgetAction(WidgetData& widgetData, sf::Time elapsedTime)
{
    WidgetState& widgetState = widgetData.state;
    const bool justOpenedFirstTime = !widgetState.wasOpenedAtLeastOnce && widgetState.wasOpened;
    const bool justOpened = !widgetState.wasOpened;

    widgetState.wasOpenedAtLeastOnce = true;
    widgetState.wasOpened = true;

    BaseMenuWidget& widget = *widgetData.widget;
    if (justOpenedFirstTime) {
        widget.OnMenuItemOpenedFirstTime();
    }
    if (justOpened) {
        widget.OnMenuItemJustOpened();
    }

    const BaseMenuWidget::MenuWidgetAction action = widget.ProcessMenuItem(elapsedTime);
    const bool shouldClose = action == BaseMenuWidget::MenuWidgetAction::ShouldClose;
    if (shouldClose) {
        widget.OnMenuItemJustClosed();
        widgetState.wasOpened = false;
    }
}
