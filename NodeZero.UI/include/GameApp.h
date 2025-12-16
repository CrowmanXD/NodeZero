#pragma once

#include <memory>
#include "Enums/GameScreen.h"
#include "raylib.h"

// Forward Declarations
class IGame;
class GameplayScreen;
class MainScreen;
class PauseScreen;
class UpgradesScreen;
class LevelCompletedScreen;
class GameoverScreen;

/**
 * @class GameApp
 * @brief The Root Application Class.
 *
 * This class owns the OS Window context (Raylib InitWindow), the main loop,
 * and the top-level State Machine (Main Menu -> Playing -> Pause).
 * It acts as the container for all specific Screens and the Core Game instance.
 */
class GameApp {
   public:
    GameApp();
    ~GameApp();

    /** @brief Starts the main application loop. Blocking call. */
    void Run();

   private:
    void Initialize();
    void Update();
    void Draw();
    void Cleanup();

    /** @brief Switches the active screen (State Machine transition). */
    void ChangeState(GameScreen newState);

    // State Management
    GameScreen m_CurrentState;
    GameScreen m_PreviousState;
    bool m_ShouldClose;

    // Core Systems
    std::unique_ptr<IGame> m_Game; // The Logic Engine

    // Screens (View Controllers)
    std::shared_ptr<GameplayScreen> m_GameplayScreen;
    std::unique_ptr<MainScreen> m_MainScreen;
    std::unique_ptr<PauseScreen> m_PauseScreen;
    std::unique_ptr<UpgradesScreen> m_UpgradesScreen;
    std::unique_ptr<LevelCompletedScreen> m_LevelCompletedScreen;
    std::unique_ptr<GameoverScreen> m_GameoverScreen;

    // Global Rendering Resources
    RenderTexture2D m_RenderTarget; // Used for Post-Processing (CRT Effect)
    Shader m_CrtShader;
    int m_ResolutionLoc;
    int m_TimeLoc;
    float m_ElapsedTime;
    Font m_Font;
    
    // Constants
    static constexpr int TARGET_FPS = 240;
    static constexpr const char* WINDOW_TITLE = "NodeZero";
    static constexpr const char* FONT_PATH = "assets/fonts/ari-w9500-display.ttf";
    static constexpr const char* SHADER_PATH = "assets/shaders/crt.fs";
};