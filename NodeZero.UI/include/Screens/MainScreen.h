#pragma once

#include <functional>
#include <memory>

#include "Enums/GameScreen.h"
#include "Widgets/Menu.h"
#include "raylib.h"

/**
 * @class MainScreen
 * @brief The initial screen displayed when the game application starts.
 *
 * Contains the main menu options (Play, Upgrades, Quit) and handles
 * state transitions based on user input.
 */
class MainScreen {
public:
    MainScreen(std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameScreen)> m_StateChangeCallback;
    Font m_Font;

    // Constants (UI Layout)
    static constexpr float TITLE_TEXT_HEIGHT_RATIO = 0.06f;
    static constexpr float TITLE_Y_RATIO = 0.3f;
    static constexpr float BUTTON_WIDTH_RATIO = 0.25f;
    static constexpr float BUTTON_HEIGHT_RATIO = 0.08f;
    static constexpr float BUTTON_SPACING_RATIO = 0.02f;

    // Watermark
    static constexpr float WATERMARK_FONT_RATIO = 0.025f;
    static constexpr float WATERMARK_X_RATIO = 0.01f;
    static constexpr float WATERMARK_Y_RATIO = 0.96f;
};