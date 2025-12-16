#pragma once

#include <functional>
#include <memory>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "Widgets/Menu.h"
#include "raylib.h"

/**
 * @class PauseScreen
 * @brief Handles the screen displayed when the game is paused.
 *
 * It draws a semi-transparent overlay and provides options to resume the game
 * or terminate the current session.
 */
class PauseScreen {
public:
    PauseScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

private:
    std::unique_ptr<Menu> m_Menu;
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;

    // Constants (UI Layout)
    static constexpr float TITLE_TEXT_HEIGHT_RATIO = 0.06f;
    static constexpr float TITLE_Y_RATIO = 0.35f;
    static constexpr float BUTTON_WIDTH_RATIO = 0.25f;
    static constexpr float BUTTON_HEIGHT_RATIO = 0.08f;
    static constexpr float BUTTON_SPACING_RATIO = 0.02f;
    static constexpr unsigned char OVERLAY_ALPHA = 150;
};