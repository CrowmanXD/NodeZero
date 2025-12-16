#pragma once

/**
 * @class ILevelService
 * @brief Interface for managing game level progression.
 *
 * This service tracks the current level number, the duration timer for the level,
 * node destruction statistics, and determines when the boss should spawn.
 */
class ILevelService {
   public:
    virtual ~ILevelService() = default;

    /**
     * @brief Initializes the service starting at a specific level.
     * @param startLevel The level to start on (must be >= 1).
     */
    virtual void Initialize(int startLevel) = 0;

    /**
     * @brief Updates the level timer.
     * @param deltaTime Time elapsed since the last frame (in seconds).
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Advances the game to the next level.
     * Resets timers and boss status.
     */
    virtual void StartNextLevel() = 0;

    /**
     * @brief Tracks that a node has been destroyed in the current level.
     * Used for statistics or unlocking achievements.
     */
    virtual void IncrementNodesDestroyed() = 0;

    /**
     * @brief Resets the service to the default state (Level 1).
     */
    virtual void Reset() = 0;

    /**
     * @brief Gets the current active level number.
     * @return The current level (1-based index).
     */
    virtual int GetCurrentLevel() const = 0;

    /**
     * @brief Gets the count of nodes destroyed in the current level.
     * @return The integer count of destroyed nodes.
     */
    virtual int GetNodesDestroyedThisLevel() const = 0;

    /**
     * @brief Calculates the completion percentage of the current level's duration.
     * @return A float between 0.0 and 100.0 representing progress.
     */
    virtual float GetProgressBarPercentage() const = 0;

    /**
     * @brief Checks if the level is fully complete (Boss defeated).
     * @return True if the level is finished, False otherwise.
     */
    virtual bool IsLevelCompleted() const = 0;

    /**
     * @brief Checks if the level timer has expired and the boss should spawn.
     * @return True if time is up and boss is not yet active.
     */
    virtual bool ShouldSpawnBoss() const = 0;
};
