#include "Screens/LevelCompletedScreen.h"

#include <string>

#include "Config/GameConfig.h"
#include "Services/IPickupService.h"
#include "Widgets/Button.h"
#include "Widgets/Label.h"

LevelCompletedScreen::LevelCompletedScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_Font(font) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Refactor: Use named constants for UI proportions
    const float buttonWidth = screenWidth * BUTTON_WIDTH_RATIO;
    const float buttonHeight = screenHeight * BUTTON_HEIGHT_RATIO;
    const float buttonSpacing = screenHeight * BUTTON_SPACING_RATIO;
    const float titleFontSize = screenHeight * TITLE_TEXT_HEIGHT_RATIO;
    const float pointsFontSize = screenHeight * POINTS_FONT_RATIO;

    float startY = screenHeight / 2.0f;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    // Title Label
    const char* titleText = "LEVEL COMPLETED!";
    Vector2 textSize = MeasureTextEx(font, titleText, titleFontSize, 1);
    float titleX = screenWidth / 2.0f - textSize.x / 2.0f;

    auto title = std::make_unique<Label>(
        titleX,
        screenHeight * TITLE_Y_RATIO,
        titleText,
        font,
        static_cast<int>(titleFontSize),
        GOLD);
    m_Menu->AddWidget(std::move(title));

    // Continue Button
    auto continueButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Continue", font);
    continueButton->SetColors(Color{ 50, 150, 50, 255 }, Color{ 80, 180, 80, 255 }, Color{ 30, 120, 30, 255 }, WHITE);
    continueButton->SetOnClick([this]() {
        m_Game.StartNextLevel();
        m_StateChangeCallback(GameScreen::Playing);
        });
    m_Menu->AddWidget(std::move(continueButton));
}

void LevelCompletedScreen::Update(float deltaTime) {
    m_Menu->Update();
}

void LevelCompletedScreen::Draw() {
    // Semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 200 });
    m_Menu->Draw();

    // Points collected text
    std::string pointsText = "Points collected: " + std::to_string(m_Game.GetPickupService().GetPickupPoints());
    int pointsFontSize = static_cast<int>(GetScreenHeight() * POINTS_FONT_RATIO);
    Vector2 textSize = MeasureTextEx(m_Font, pointsText.c_str(), static_cast<float>(pointsFontSize), 1);

    DrawTextEx(m_Font, pointsText.c_str(),
        Vector2{ GetScreenWidth() / 2.0f - textSize.x / 2.0f, GetScreenHeight() * POINTS_Y_RATIO },
        static_cast<float>(pointsFontSize), 1, WHITE);
}