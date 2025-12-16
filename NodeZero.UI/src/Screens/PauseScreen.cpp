#include "Screens/PauseScreen.h"

#include "Config/GameConfig.h"
#include "Widgets/Button.h"
#include "Widgets/Label.h"

PauseScreen::PauseScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Refactor: Use named constants for UI proportions
    const float buttonWidth = screenWidth * BUTTON_WIDTH_RATIO;
    const float buttonHeight = screenHeight * BUTTON_HEIGHT_RATIO;
    const float buttonSpacing = screenHeight * BUTTON_SPACING_RATIO;
    const float titleFontSize = screenHeight * TITLE_TEXT_HEIGHT_RATIO;

    float startY = screenHeight / 2.0f - buttonHeight * 0.5f;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    // Title Label
    const char* titleText = "GAME PAUSED";
    Vector2 textSize = MeasureTextEx(font, titleText, titleFontSize, 1);
    float titleX = screenWidth / 2.0f - textSize.x / 2.0f;

    auto pauseTitle = std::make_unique<Label>(
        titleX,
        screenHeight * TITLE_Y_RATIO,
        titleText,
        font,
        static_cast<int>(titleFontSize),
        WHITE);
    m_Menu->AddWidget(std::move(pauseTitle));

    // Resume Button
    auto resumeButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Resume", font);
    resumeButton->SetColors(Color{ 50, 150, 50, 255 }, Color{ 80, 180, 80, 255 }, Color{ 30, 120, 30, 255 }, WHITE);
    resumeButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Playing); });
    m_Menu->AddWidget(std::move(resumeButton));

    // Terminate Button
    auto terminateButton = std::make_unique<Button>(centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, "Terminate", font);
    terminateButton->SetColors(Color{ 180, 50, 50, 255 }, Color{ 210, 80, 80, 255 }, Color{ 150, 30, 30, 255 }, WHITE);
    terminateButton->SetOnClick([this]() {
        m_Game.SaveProgress();
        m_StateChangeCallback(GameScreen::GameOver);
        });
    m_Menu->AddWidget(std::move(terminateButton));
}

void PauseScreen::Update(float deltaTime) {
    m_Menu->Update();
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::Playing);
    }
}

void PauseScreen::Draw() {
    // Draw semi-transparent black overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, OVERLAY_ALPHA });
    m_Menu->Draw();
}