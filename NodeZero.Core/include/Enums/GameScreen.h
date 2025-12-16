#pragma once

/**
 * @enum GameScreen
 * @brief Represents the current UI/State of the application.
 *
 * Dictates which "Screen" class is currently updating and rendering.
 */
enum class GameScreen {
    /** @brief The starting screen with options to start game or quit. */
    MainMenu,

    /** @brief The main active gameplay loop. */
    Playing,

    /** @brief The game loop is frozen, showing the pause menu. */
    Paused,

    /** @brief The screen shown between levels (Success state). */
    LevelCompleted,

    /** @brief The screen shown when the player dies (Failure state). */
    GameOver,

    /** @brief The shop/upgrade screen. */
    Upgrades,

    /** @brief Signal to the engine to close the application window. */
    Quit
};