#pragma once

#include <functional>
#include <memory>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "Widgets/Menu.h"
#include "raylib.h"

/**
 * @class GameoverScreen
 * @brief Handles the display and user interaction logic for the Game Over screen.
 *
 * This screen displays the final score and provides options to restart or return
 * to the main menu, resetting game state upon interaction.
 */
class GameoverScreen {
public:
    GameoverScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    /**
     * @brief Handles widget updates (e.g., button hover/click).
     */
    void Update(float deltaTime);

    /**
     * @brief Draws the "Game Over" title, final score, and menu buttons.
     */
    void Draw();

private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::unique_ptr<Menu> m_Menu;
    Font m_Font;

    // --- Constants (UI Layout) ---
    static constexpr float TITLE_TEXT_HEIGHT_RATIO = 0.08f;
    static constexpr float BUTTON_WIDTH_RATIO = 0.25f;
    static constexpr float BUTTON_HEIGHT_RATIO = 0.08f;
    static constexpr float BUTTON_SPACING_RATIO = 0.02f;
    static constexpr float TITLE_Y_RATIO = 0.3f;
    static constexpr float POINTS_Y_RATIO = 0.42f;
    static constexpr float POINTS_FONT_RATIO = 0.04f;
};