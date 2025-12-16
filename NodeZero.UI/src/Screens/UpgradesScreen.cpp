#include "Screens/UpgradesScreen.h"

#include <cstdio>

#include "Services/IUpgradeService.h"
#include "Services/ISaveService.h"

UpgradesScreen::UpgradesScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_WasMousePressed(false), m_IsFirstFrame(true), m_Font(font) {
}

void UpgradesScreen::Update(float deltaTime) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::MainMenu);
        m_IsFirstFrame = true; // Reset mouse state tracking
    }
}

void UpgradesScreen::Draw() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Title
    int titleFontSize = static_cast<int>(screenHeight * TITLE_TEXT_HEIGHT_RATIO);
    Vector2 titleSize = MeasureTextEx(m_Font, "UPGRADES", static_cast<float>(titleFontSize), 1);
    DrawTextEx(m_Font, "UPGRADES",
        Vector2{ static_cast<float>(screenWidth / 2 - titleSize.x / 2),
                static_cast<float>(screenHeight * TITLE_Y_RATIO) },
        static_cast<float>(titleFontSize), 1, WHITE);

    SaveData saveData = m_Game.GetSaveService().GetCurrentData();

    // Statistics Column Layout
    int statsX = static_cast<int>(screenWidth * STATS_X_RATIO);
    int statsY = static_cast<int>(screenHeight * STATS_Y_RATIO);
    int statsSpacing = static_cast<int>(screenHeight * STATS_SPACING_RATIO);
    int subtitleFontSize = static_cast<int>(screenHeight * STATS_SUBTITLE_FONT_RATIO);
    int statTextFontSize = static_cast<int>(screenHeight * STATS_TEXT_FONT_RATIO);

    // Subtitle
    DrawTextEx(m_Font, "STATISTICS", Vector2{ static_cast<float>(statsX), static_cast<float>(statsY) }, static_cast<float>(subtitleFontSize), 1, YELLOW);

    // Draw Statistics
    int currentStatsY = statsY + statsSpacing;
    char buffer[64];

    snprintf(buffer, sizeof(buffer), "Points: %d", saveData.points);
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, Color{ 255, 215, 0, 255 });
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "High Points: %d", saveData.highPoints);
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Games Played: %d", saveData.gamesPlayed);
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Total Nodes: %d", saveData.totalNodesDestroyed);
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Max Health: %.0f", m_Game.GetUpgradeService().GetMaxHealth());
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Regen Rate: %.1f/s", m_Game.GetUpgradeService().GetRegenRate());
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Damage Zone: %.0f", m_Game.GetUpgradeService().GetDamageZoneSize());
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;

    snprintf(buffer, sizeof(buffer), "Damage/Tick: %.0f", m_Game.GetUpgradeService().GetDamagePerTick());
    DrawTextEx(m_Font, buffer, Vector2{ static_cast<float>(statsX), static_cast<float>(currentStatsY) }, static_cast<float>(statTextFontSize), 1, WHITE);
    currentStatsY += statsSpacing;


    // Upgrade Column Layout
    int upgradeTitleFontSize = static_cast<int>(screenHeight * UPGRADE_TITLE_FONT_RATIO);
    int buttonWidth = static_cast<int>(screenWidth * BUTTON_WIDTH_RATIO);
    int buttonHeight = static_cast<int>(screenHeight * BUTTON_HEIGHT_RATIO);
    int buttonSpacing = static_cast<int>(screenHeight * BUTTON_SPACING_RATIO);
    int buttonTextFontSize = static_cast<int>(buttonHeight * BUTTON_TEXT_FONT_RATIO);
    int costTextFontSize = static_cast<int>(buttonHeight * COST_TEXT_FONT_RATIO);

    int buttonX = screenWidth / 2 - buttonWidth / 2;
    int upgradeY = static_cast<int>(screenHeight * UPGRADE_Y_START_RATIO);

    // Upgrade Column Title
    Vector2 upgradesTitleSize = MeasureTextEx(m_Font, "UPGRADES", static_cast<float>(upgradeTitleFontSize), 1);
    DrawTextEx(m_Font, "UPGRADES",
        Vector2{ static_cast<float>(screenWidth / 2 - upgradesTitleSize.x / 2),
                static_cast<float>(upgradeY - screenHeight * 0.05f) },
        static_cast<float>(upgradeTitleFontSize), 1, Color{ 100, 200, 255, 255 });

    Vector2 mousePos = GetMousePosition();
    bool isMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    int currentButtonY = upgradeY + static_cast<int>(screenHeight * 0.01f);

    // Lambda to handle drawing and clicking a single button
    auto drawAndClickUpgradeButton = [&](int& currentY, const char* upgradeText, int cost, bool isMaxed, const std::function<bool()>& buyAction) {
        Rectangle buttonRect = { static_cast<float>(buttonX), static_cast<float>(currentY),
                                static_cast<float>(buttonWidth), static_cast<float>(buttonHeight) };
        bool isHovered = CheckCollisionPointRec(mousePos, buttonRect);

        bool canAfford = saveData.points >= cost && !isMaxed;
        Color buttonColor = isMaxed
            ? Color{ 50, 50, 50, 255 }
            : (canAfford ? (isHovered ? Color{ 80, 180, 80, 255 } : Color{ 60, 160, 60, 255 })
                : Color{ 100, 100, 100, 255 });

        DrawRectangleRec(buttonRect, buttonColor);
        DrawRectangleLinesEx(buttonRect, 1, WHITE);

        // Button Text
        char buttonText[64];
        snprintf(buttonText, sizeof(buttonText), "%s", isMaxed ? "MAX LEVEL" : upgradeText);
        Vector2 buttonTextSize = MeasureTextEx(m_Font, buttonText, static_cast<float>(buttonTextFontSize), 1);
        DrawTextEx(m_Font, buttonText,
            Vector2{ static_cast<float>(buttonX + buttonWidth / 2 - buttonTextSize.x / 2),
                    static_cast<float>(currentY + buttonHeight * BUTTON_TEXT_Y_RATIO) },
            static_cast<float>(buttonTextFontSize), 1, WHITE);

        // Cost Text
        char costText[32];
        snprintf(costText, sizeof(costText), "%s", isMaxed ? "Maxed out" : TextFormat("Cost: %d points", cost));
        Vector2 costTextSize = MeasureTextEx(m_Font, costText, static_cast<float>(costTextFontSize), 1);
        DrawTextEx(m_Font, costText,
            Vector2{ static_cast<float>(buttonX + buttonWidth / 2 - costTextSize.x / 2),
                    static_cast<float>(currentY + buttonHeight * COST_TEXT_Y_RATIO) },
            static_cast<float>(costTextFontSize), 1, LIGHTGRAY);

        // Click Logic
        if (!m_IsFirstFrame && isHovered && canAfford && isMousePressed && !m_WasMousePressed) {
            if (buyAction()) {
                // Refresh data immediately after a successful purchase
                saveData = m_Game.GetSaveService().GetCurrentData();
            }
        }

        // Move Y position for next button
        currentY += buttonHeight + buttonSpacing;
        };


    // Health Upgrade
    drawAndClickUpgradeButton(currentButtonY, "Upgrade +1.0 HP", m_Game.GetUpgradeService().GetHealthUpgradeCost(), false,
        [this]() { return m_Game.GetUpgradeService().BuyHealthUpgrade(); }
    );

    // Regen Upgrade
    drawAndClickUpgradeButton(currentButtonY, "Upgrade +0.1 Regen", m_Game.GetUpgradeService().GetRegenUpgradeCost(), false,
        [this]() { return m_Game.GetUpgradeService().BuyRegenUpgrade(); }
    );

    // Damage Zone Upgrade
    bool isDamageZoneMaxed = m_Game.GetUpgradeService().GetDamageZoneSize() >= 300.0f;
    drawAndClickUpgradeButton(currentButtonY, "Upgrade +10 Zone", m_Game.GetUpgradeService().GetDamageZoneUpgradeCost(), isDamageZoneMaxed,
        [this]() { return m_Game.GetUpgradeService().BuyDamageZoneUpgrade(); }
    );

    // Damage Upgrade
    drawAndClickUpgradeButton(currentButtonY, "Upgrade +5 Damage", m_Game.GetUpgradeService().GetDamageUpgradeCost(), false,
        [this]() { return m_Game.GetUpgradeService().BuyDamageUpgrade(); }
    );


    // Footer
    int escTextFontSize = static_cast<int>(screenHeight * FOOTER_TEXT_FONT_RATIO);
    Vector2 escTextSize = MeasureTextEx(m_Font, "Press ESC to return to menu", static_cast<float>(escTextFontSize), 1);
    DrawTextEx(m_Font, "Press ESC to return to menu",
        Vector2{ static_cast<float>(screenWidth / 2 - escTextSize.x / 2),
                static_cast<float>(screenHeight * FOOTER_Y_RATIO) },
        static_cast<float>(escTextFontSize), 1, LIGHTGRAY);

    // Update mouse state tracking
    m_WasMousePressed = isMousePressed;
    m_IsFirstFrame = false;
}