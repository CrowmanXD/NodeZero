#pragma once

#include <functional>
#include <memory>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "Widgets/Menu.h"
#include "raylib.h"

/**
 * @class LevelCompletedScreen
 * @brief Handles the screen displayed upon successful completion of a level.
 *
 * It shows collected points for the level and provides a button to continue,
 * which triggers the game to prepare for the next level.
 */
class LevelCompletedScreen {
public:
    LevelCompletedScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);
    void Update(float deltaTime);
    void Draw();

private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::unique_ptr<Menu> m_Menu;
    Font m_Font;

    // Constants (UI Layout)
    static constexpr float TITLE_TEXT_HEIGHT_RATIO = 0.06f;
    static constexpr float TITLE_Y_RATIO = 0.3f;
    static constexpr float POINTS_FONT_RATIO = 0.04f;
    static constexpr float POINTS_Y_RATIO = 0.42f;
    static constexpr float BUTTON_WIDTH_RATIO = 0.25f;
    static constexpr float BUTTON_HEIGHT_RATIO = 0.08f;
    static constexpr float BUTTON_SPACING_RATIO = 0.02f;
};