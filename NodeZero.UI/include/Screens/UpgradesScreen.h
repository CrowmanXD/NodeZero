#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "raylib.h"

/**
 * @class UpgradesScreen
 * @brief Handles the screen where the player can spend points to purchase upgrades.
 *
 * Draws current statistics and provides interactive buttons for purchasing
 * persistent upgrades (Health, Regen, Damage, Zone Size). Handles mouse click
 * debouncing and purchase logic.
 */
class UpgradesScreen {
public:
    UpgradesScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;

    /** @brief Tracks mouse state to prevent immediate double-clicks. */
    bool m_WasMousePressed;

    /** @brief Ensures the first frame's mouse state is ignored. */
    bool m_IsFirstFrame;
    Font m_Font;

    // Constants (UI Layout)
    static constexpr float TITLE_TEXT_HEIGHT_RATIO = 0.05f;
    static constexpr float TITLE_Y_RATIO = 0.05f;

    // Statistics Column
    static constexpr float STATS_X_RATIO = 0.03f;
    static constexpr float STATS_Y_RATIO = 0.15f;
    static constexpr float STATS_SUBTITLE_FONT_RATIO = 0.035f;
    static constexpr float STATS_TEXT_FONT_RATIO = 0.025f;
    static constexpr float STATS_SPACING_RATIO = 0.04f;

    // Upgrade Buttons Column
    static constexpr float UPGRADE_TITLE_FONT_RATIO = 0.038f;
    static constexpr float UPGRADE_Y_START_RATIO = 0.4f;
    static constexpr float BUTTON_WIDTH_RATIO = 0.2f;
    static constexpr float BUTTON_HEIGHT_RATIO = 0.07f;
    static constexpr float BUTTON_SPACING_RATIO = 0.01f;
    static constexpr float BUTTON_TEXT_FONT_RATIO = 0.35f;
    static constexpr float COST_TEXT_FONT_RATIO = 0.28f;
    static constexpr float BUTTON_TEXT_Y_RATIO = 0.15f;
    static constexpr float COST_TEXT_Y_RATIO = 0.55f;

    // Footer
    static constexpr float FOOTER_TEXT_FONT_RATIO = 0.025f;
    static constexpr float FOOTER_Y_RATIO = 0.95f;
};