#include "menu_root_widget.h"

void MenuRootWidget::UpdateWidget(sf::Time elapsedTime)
{
    if (ImGui::BeginMainMenuBar()) {
        if (const WidgetsGroup* rootGroup = FindWidgetGroup(_rootWidgetId)) {
            for (const MenuWidgetId id : rootGroup->items) {
                const bool shouldCloseGroup = UpdateWidgetsGroup(id, elapsedTime);
                if (shouldCloseGroup) {
                    break;
                }
            }
        }
        ImGui::EndMainMenuBar();
    }
}

MenuWidgetId MenuRootWidget::AddWidget(MenuWidgetId parent, WidgetData widgetData)
{
    const auto childId = static_cast<MenuWidgetId>(_widgets.size());
    ASSERT(childId != RootWidgetId, "Limit of widgets reached!");
    _widgets.emplace_back(std::move(widgetData));
    _indexedGroups[parent].items.emplace_back(childId);
    return childId;
}

MenuRootWidget::WidgetData& MenuRootWidget::ModifyWidgetData(MenuWidgetId id)
{
    const size_t index = static_cast<size_t>(id);
    ASSERT(index < _widgets.size());
    return _widgets[index];
}

const MenuRootWidget::WidgetsGroup* MenuRootWidget::FindWidgetGroup(MenuWidgetId id) const
{
    const auto groupIt = _indexedGroups.find(id);
    if (groupIt == _indexedGroups.end()) {
        return nullptr;
    }
    const WidgetsGroup& group = groupIt->second;
    if (group.items.empty()) {
        return nullptr;
    }
    return &group;
}

bool MenuRootWidget::UpdateWidgetsGroup(MenuWidgetId id, sf::Time elapsedTime)
{
    const WidgetsGroup* group = FindWidgetGroup(id);
    WidgetData& widgetData = ModifyWidgetData(id);
    if (!group) {
        return ProcessWidgetAction(widgetData, elapsedTime);
    }

    bool shouldCloseGroup = false;
    for (const MenuWidgetId childId : group->items) {
        if (!ImGui::BeginMenu(widgetData.name.c_str())) {
            continue;
        }
        shouldCloseGroup = UpdateWidgetsGroup(childId, elapsedTime);
        ImGui::EndMenu();
        if (shouldCloseGroup) {
            widgetData.state.wasOpened = false;
            break;
        }
    }
    return shouldCloseGroup;
}

bool MenuRootWidget::ProcessWidgetAction(WidgetData& widgetData, sf::Time elapsedTime)
{
    if (!ImGui::MenuItem(widgetData.name.c_str())) {
        return false;
    }

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
    return shouldClose;
}
