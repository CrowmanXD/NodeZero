#include "Screens/MainScreen.h"

#include "Config/GameConfig.h"
#include "Widgets/Button.h"
#include "Widgets/Label.h"

MainScreen::MainScreen(std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_StateChangeCallback(stateChangeCallback), m_Font(font) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Refactor: Use named constants for UI proportions
    const float buttonWidth = screenWidth * BUTTON_WIDTH_RATIO;
    const float buttonHeight = screenHeight * BUTTON_HEIGHT_RATIO;
    const float buttonSpacing = screenHeight * BUTTON_SPACING_RATIO;
    const float titleFontSize = screenHeight * TITLE_TEXT_HEIGHT_RATIO;
    const float watermarkFontSize = screenHeight * WATERMARK_FONT_RATIO;

    // Button layout starting point (centered, slightly above middle)
    float startY = screenHeight / 2.0f - buttonHeight;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    // Title Label
    const char* titleText = "NodeZero";
    Vector2 textSize = MeasureTextEx(font, titleText, titleFontSize, 1);
    float titleX = screenWidth / 2.0f - textSize.x / 2.0f;

    auto titleLabel = std::make_unique<Label>(
        titleX,
        screenHeight * TITLE_Y_RATIO,
        titleText,
        font,
        static_cast<int>(titleFontSize),
        WHITE);
    m_Menu->AddWidget(std::move(titleLabel));

    // Play Button
    auto playButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Play", font);
    playButton->SetColors(Color{ 70, 130, 180, 255 }, Color{ 100, 160, 210, 255 }, Color{ 40, 100, 150, 255 }, WHITE);
    playButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Playing); });
    m_Menu->AddWidget(std::move(playButton));

    // Upgrades Button
    auto upgradesButton = std::make_unique<Button>(centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, "Upgrades", font);
    upgradesButton->SetColors(Color{ 50, 150, 50, 255 }, Color{ 80, 180, 80, 255 }, Color{ 30, 120, 30, 255 }, WHITE);
    upgradesButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Upgrades); });
    m_Menu->AddWidget(std::move(upgradesButton));

    // Quit Button
    auto quitButton = std::make_unique<Button>(centerX, startY + (buttonHeight + buttonSpacing) * 2, buttonWidth, buttonHeight, "Quit", font);
    quitButton->SetColors(Color{ 180, 70, 70, 255 }, Color{ 210, 100, 100, 255 }, Color{ 150, 40, 40, 255 }, WHITE);
    quitButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Quit); });
    m_Menu->AddWidget(std::move(quitButton));
}

void MainScreen::Update(float deltaTime) {
    m_Menu->Update();
}

void MainScreen::Draw() {
    m_Menu->Draw();

    // Watermark
    const char* watermarkText = "Made by TeamTBD";
    DrawTextEx(m_Font, watermarkText,
        Vector2{
            static_cast<float>(GetScreenWidth() * WATERMARK_X_RATIO),
            static_cast<float>(GetScreenHeight() * WATERMARK_Y_RATIO)
        },
        static_cast<float>(GetScreenHeight() * WATERMARK_FONT_RATIO),
        1,
        Color{ 150, 150, 150, 200 });
}