#pragma once
#include "game_config.h"
#include "menu_widgets/base/base_menu_widget.h"

class GameController;
class GameWidget final : public BaseMenuWidget {
public:
    GameWidget(GameController& gameController, GameConfig& gameConfig);
    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    void ProcessFireworksConfig(GameConfig::FireworksConfig& config);

    GameController* _gameController { nullptr };
    GameConfig* _gameConfig { nullptr };
};