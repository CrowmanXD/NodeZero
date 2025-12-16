/**
 * @mainpage NodeZero Documentation
 * * @section intro_sec Introduction
 * Welcome to the NodeZero project documentation.
 */

/**
 * @file main.cpp
 * @brief Entry point for the NodeZero game application.
 *
 * Contains the standard C++ main function which bootstraps the
 * GameApp wrapper and enters the primary execution loop.
 */

#include "GameApp.h"

 /**
  * @brief Application entry point.
  *
  * Instantiates the GameApp class, which manages the operating system window,
  * loads global resources (fonts, shaders), and orchestrates the high-level
  * state machine (Menu -> Game -> GameOver).
  *
  * @return 0 upon successful execution and clean shutdown.
  */
int main() {
    // Create the application instance (RAII: Resources initialized in constructor)
    GameApp app;

    // Start the game loop (Block until window closes or Quit is selected)
    app.Run();

    return 0;
}