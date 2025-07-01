#include "menu_root_widget.h"

bool MenuRootWidget::HasWidget(MenuWidgetId id) const
{
    if (id == MenuWidgetId::Root) {
        return true;
    }
    return std::to_underlying(id) < _widgets.size();
}

void MenuRootWidget::OpenWidget(MenuWidgetId id)
{
    if (!HasWidget(id)) {
        ASSERT_FAIL("Invalid widget id!");
        return;
    }
    ChangeWidgetState(id, true, GetWidgetData(id).state.opened);
}

void MenuRootWidget::CloseWidget(MenuWidgetId id)
{
    if (!HasWidget(id)) {
        ASSERT_FAIL("Invalid widget id!");
        return;
    }
    ChangeWidgetState(id, false, GetWidgetData(id).state.opened);
}

void MenuRootWidget::UpdateWidget(Common::Time elapsedTime)
{
    if (ImGui::BeginMainMenuBar()) {
        if (const WidgetsGroup* rootGroup = FindWidgetGroup(MenuWidgetId::Root)) {
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
    if (_widgets.size() >= LimitNumberOfWidgets) {
        ASSERT_FAIL("Limit of widgets reached!");
        return MenuWidgetId::Invalid;
    }
    if (!HasWidget(parent)) {
        ASSERT_FAIL("Invalid parent id!");
        return MenuWidgetId::Invalid;
    }
    const auto childId = static_cast<MenuWidgetId>(_widgets.size());
    _widgets.emplace_back(std::move(widgetData));
    _indexedGroups[parent].items.emplace_back(childId);
    return childId;
}

MenuRootWidget::WidgetData& MenuRootWidget::ModifyWidgetData(MenuWidgetId id)
{
    const size_t index = static_cast<size_t>(id);
    ASSERT(index < _widgets.size(), "Invalid index!");
    return _widgets[index];
}

const MenuRootWidget::WidgetData& MenuRootWidget::GetWidgetData(MenuWidgetId id) const
{
    const size_t index = static_cast<size_t>(id);
    ASSERT(index < _widgets.size(), "Invalid index!");
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

void MenuRootWidget::UpdateWidgetsGroup(MenuWidgetId id)
{
    const WidgetsGroup* group = FindWidgetGroup(id);
    if (!group) {
        ProcessWidgetState(id);
        return;
    }

    for (const MenuWidgetId childId : group->items) {
        const WidgetData& widgetData = GetWidgetData(id);
        if (!ImGui::BeginMenu(widgetData.name.c_str())) {
            continue;
        }
        UpdateWidgetsGroup(childId);
        ImGui::EndMenu();
    }
}

void MenuRootWidget::ProcessWidgetState(MenuWidgetId id)
{
    const WidgetData& widgetData = GetWidgetData(id);
    const WidgetState& widgetState = widgetData.state;

    const bool wasOpen = widgetState.opened;
    bool opened = widgetState.opened;
    const bool openedByImgui = ImGui::MenuItem(widgetData.name.c_str(), nullptr, &opened);
    if (!openedByImgui) {
        return;
    }

    ChangeWidgetState(id, opened, wasOpen);
}

void MenuRootWidget::ChangeWidgetState(MenuWidgetId id, bool openedNow, bool wasOpen)
{
    WidgetData& widgetData = ModifyWidgetData(id);
    WidgetState& widgetState = widgetData.state;

    if (openedNow) {
        widgetState.opened = true;
        if (!widgetState.wasOpenedAtLeastOnce) {
            widgetState.wasOpenedAtLeastOnce = true;
            widgetState.justOpenedFirstTime = true;
        }
        widgetState.justOpened = !wasOpen;
        if (widgetState.justOpened) {
            _openedWidgets.emplace_back(id);
        }
    } else {
        widgetState.opened = false;
        widgetState.justClosed = wasOpen && !widgetState.opened;
    }
}

void MenuRootWidget::UpdateOpenedWidgets(Common::Time elapsedTime)
{
    auto it = std::ranges::remove_if(_openedWidgets, [this, elapsedTime](const MenuWidgetId id) {
        const bool shouldClose = ProcessOpenedWidgetState(id, elapsedTime);
        return shouldClose;
    });
    _openedWidgets.erase(it.begin(), it.end());
}

bool MenuRootWidget::ProcessOpenedWidgetState(const MenuWidgetId id, const Common::Time elapsedTime)
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
    if (widgetState.opened) {
        bool isOpened{true};
        ImGui::Begin(widgetData.name.c_str(), &isOpened);
        BaseMenuWidget::MenuWidgetAction action = BaseMenuWidget::MenuWidgetAction::ShouldClose;
        if (isOpened) {
            action = widget.ProcessMenuItem(elapsedTime);
        }
        ImGui::End();
        widgetState.justClosed = action == BaseMenuWidget::MenuWidgetAction::ShouldClose;
    }
    if (widgetState.justClosed) {
        widgetState.justClosed = false;
        widgetState.opened = false;
        widget.OnMenuItemJustClosed();
        return true;
    }
    return false;
}
