#include "menu_root_widget.h"

void MenuRootWidget::UpdateWidget(sf::Time elapsedTime)
{
    if (ImGui::BeginMainMenuBar()) {
        if (const WidgetsGroup* rootGroup = FindWidgetGroup(_rootWidgetId)) {
            for (const MenuWidgetId id : rootGroup->items) {
                UpdateWidgetsGroup(id);
            }
        }
        ImGui::EndMainMenuBar();
    }

    UpdateOpenedWidgets(elapsedTime);
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

const MenuRootWidget::WidgetData& MenuRootWidget::GetWidgetData(MenuWidgetId id) const
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

bool MenuRootWidget::UpdateWidgetsGroup(MenuWidgetId id)
{
    const WidgetsGroup* group = FindWidgetGroup(id);
    if (!group) {
        return ProcessWidgetState(id);
    }

    bool shouldProcessChild = false;
    for (const MenuWidgetId childId : group->items) {
        const WidgetData& widgetData = GetWidgetData(id);
        if (!ImGui::BeginMenu(widgetData.name.c_str())) {
            continue;
        }
        shouldProcessChild = UpdateWidgetsGroup(childId);
        ImGui::EndMenu();
    }
    return shouldProcessChild;
}

bool MenuRootWidget::ProcessWidgetState(MenuWidgetId id)
{
    WidgetData& widgetData = ModifyWidgetData(id);
    WidgetState& widgetState = widgetData.state;

    const bool wasOpen = widgetState.opened;

    if (ImGui::MenuItem(widgetData.name.c_str(), nullptr, &widgetState.opened)) {
        if (!widgetState.wasOpenedAtLeastOnce) {
            widgetState.wasOpenedAtLeastOnce = true;
            widgetState.justOpenedFirstTime = true;
        }
        widgetState.justOpened = !wasOpen;
        if (widgetState.justOpened) {
            _openedWidgets.emplace_back(id);
        }
        return true;
    }

    return false;
}
void MenuRootWidget::UpdateOpenedWidgets(sf::Time elapsedTime)
{
    auto it = std::ranges::remove_if(_openedWidgets, [this, elapsedTime](const MenuWidgetId id) {
        return ProcessOpenedWidgetState(id, elapsedTime);
    });
    _openedWidgets.erase(it.begin(), it.end());
}

bool MenuRootWidget::ProcessOpenedWidgetState(const MenuWidgetId id, sf::Time elapsedTime)
{
    WidgetData& widgetData = ModifyWidgetData(id);
    BaseMenuWidget& widget = *widgetData.widget;
    WidgetState& widgetState = widgetData.state;

    if (widgetState.justOpenedFirstTime) {
        widgetState.justOpenedFirstTime = false;
        widget.OnMenuItemOpenedFirstTime();
    }
    if (widgetState.justOpened) {
        widgetState.justOpened = false;
        widget.OnMenuItemJustOpened();
    }

    const BaseMenuWidget::MenuWidgetAction action = widget.ProcessMenuItem(elapsedTime);
    widgetState.opened = action == BaseMenuWidget::MenuWidgetAction::KeepOpen;

    if (!widgetState.opened) {
        widget.OnMenuItemJustClosed();
    }

    return !widgetState.opened;
}
