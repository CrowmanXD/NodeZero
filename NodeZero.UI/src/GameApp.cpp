#include "GameApp.h"

#include <algorithm>
#include <iostream>

#include "Config/GameConfig.h"
#include "EventLogger.h"
#include "Game.h"
#include "IGame.h"
#include "InputHandler.h"
#include "Renderer.h"

// Screen Includes
#include "Screens/GameoverScreen.h"
#include "Screens/GameplayScreen.h"
#include "Screens/LevelCompletedScreen.h"
#include "Screens/MainScreen.h"
#include "Screens/PauseScreen.h"
#include "Screens/UpgradesScreen.h"

#include "raymath.h"

GameApp::GameApp()
    : m_CurrentState(GameScreen::MainMenu),
    m_PreviousState(GameScreen::MainMenu),
    m_ShouldClose(false),
    m_ElapsedTime(0.0f),
    m_ResolutionLoc(0),
    m_TimeLoc(0) {
}

GameApp::~GameApp() {
    Cleanup();
}

void GameApp::Initialize() {
    // Window Setup
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0, 0, WINDOW_TITLE);

    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);

    SetWindowSize(width, height);
    SetWindowPosition(0, 0);

    SetExitKey(KEY_NULL); // Disable ESC to quit (we handle it manually)
    SetTargetFPS(TARGET_FPS);

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Resource Loading
    m_Font = LoadFont(FONT_PATH);

    // Logic Initialization
    m_Game = std::make_unique<Game>();
    m_Game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Observer Attachment
    auto eventLogger = std::make_shared<EventLogger>();
    m_Game->Attach(eventLogger);

    // Screen Setup
    auto stateChangeCallback = [this](GameScreen newState) { ChangeState(newState); };

    m_MainScreen = std::make_unique<MainScreen>(stateChangeCallback, m_Font);
    m_GameplayScreen = std::make_shared<GameplayScreen>(*m_Game, stateChangeCallback, m_Font);
    m_PauseScreen = std::make_unique<PauseScreen>(*m_Game, stateChangeCallback, m_Font);
    m_UpgradesScreen = std::make_unique<UpgradesScreen>(*m_Game, stateChangeCallback, m_Font);
    m_LevelCompletedScreen = std::make_unique<LevelCompletedScreen>(*m_Game, stateChangeCallback, m_Font);
    m_GameoverScreen = std::make_unique<GameoverScreen>(*m_Game, stateChangeCallback, m_Font);

    // Gameplay Screen needs to listen to events (shake, particles)
    m_Game->Attach(m_GameplayScreen);

    // Shader Setup (Post-Processing)
    m_RenderTarget = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureWrap(m_RenderTarget.texture, TEXTURE_WRAP_CLAMP);

    m_CrtShader = LoadShader(0, SHADER_PATH);

    m_ResolutionLoc = GetShaderLocation(m_CrtShader, "resolution");
    m_TimeLoc = GetShaderLocation(m_CrtShader, "time");

    float resolution[2] = { static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
    SetShaderValue(m_CrtShader, m_ResolutionLoc, resolution, SHADER_UNIFORM_VEC2);
}

void GameApp::ChangeState(GameScreen newState) {
    m_CurrentState = newState;
}

void GameApp::Run() {
    Initialize();

    while (!WindowShouldClose() && m_CurrentState != GameScreen::Quit) {
        Update();
        Draw();
    }
}

void GameApp::Update() {
    float deltaTime = GetFrameTime();
    m_ElapsedTime += deltaTime;

    SetShaderValue(m_CrtShader, m_TimeLoc, &m_ElapsedTime, SHADER_UNIFORM_FLOAT);

    if (m_CurrentState == GameScreen::Playing && m_PreviousState != GameScreen::Playing) {
        HideCursor();
        if (m_PreviousState == GameScreen::GameOver || m_PreviousState == GameScreen::MainMenu) {
            m_GameplayScreen->ClearEffects();
        }
        m_Game->GetHealthService().SetMaxHealth(m_Game->GetUpgradeService().GetMaxHealth());
        m_Game->GetHealthService().SetRegenRate(m_Game->GetUpgradeService().GetRegenRate());
        m_Game->GetHealthService().RestoreToMax();
    }
    else if (m_CurrentState != GameScreen::Playing && m_PreviousState == GameScreen::Playing) {
        ShowCursor();
    }
    m_PreviousState = m_CurrentState;

    switch (m_CurrentState) {
    case GameScreen::MainMenu:
        m_MainScreen->Update(deltaTime);
        break;
    case GameScreen::Playing:
        m_GameplayScreen->Update(deltaTime);
        break;
    case GameScreen::Paused:
        m_PauseScreen->Update(deltaTime);
        break;
    case GameScreen::LevelCompleted:
        m_LevelCompletedScreen->Update(deltaTime);
        break;
    case GameScreen::Upgrades:
        m_UpgradesScreen->Update(deltaTime);
        break;
    case GameScreen::GameOver:
        m_GameoverScreen->Update(deltaTime);
        break;
    case GameScreen::Quit:
        break;
    }
}

void GameApp::Draw() {
    // Draw Game Content to Offscreen Buffer
    BeginTextureMode(m_RenderTarget);
    ClearBackground(Color{ 40, 40, 40, 255 });

    // Draw gameplay layer in background for certain screens (transparency overlay)
    bool drawGameplayBg = (m_CurrentState == GameScreen::Playing ||
        m_CurrentState == GameScreen::Paused ||
        m_CurrentState == GameScreen::LevelCompleted ||
        m_CurrentState == GameScreen::GameOver);

    if (drawGameplayBg) {
        m_GameplayScreen->Draw();
    }

    // Draw active UI
    switch (m_CurrentState) {
    case GameScreen::Playing:        break; // Already drawn above
    case GameScreen::Paused:         m_PauseScreen->Draw(); break;
    case GameScreen::LevelCompleted: m_LevelCompletedScreen->Draw(); break;
    case GameScreen::MainMenu:       m_MainScreen->Draw(); break;
    case GameScreen::Upgrades:       m_UpgradesScreen->Draw(); break;
    case GameScreen::GameOver:       m_GameoverScreen->Draw(); break;
    case GameScreen::Quit:           break;
    }

    EndTextureMode();

    // 2. Draw Buffer to Screen with Shader
    BeginDrawing();
    ClearBackground(BLACK);

    BeginShaderMode(m_CrtShader);
    // Draw texture flipped vertically because of OpenGL coordinates
    DrawTextureRec(
        m_RenderTarget.texture,
        Rectangle{ 0, 0, static_cast<float>(m_RenderTarget.texture.width), static_cast<float>(-m_RenderTarget.texture.height) },
        Vector2{ 0, 0 },
        WHITE);
    EndShaderMode();

    EndDrawing();
}

void GameApp::Cleanup() {
    UnloadFont(m_Font);
    UnloadShader(m_CrtShader);
    UnloadRenderTexture(m_RenderTarget);
    ShowCursor();
    CloseWindow();
}