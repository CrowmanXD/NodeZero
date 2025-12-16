#pragma once

#include "Services/ILevelService.h"

/**
 * @class LevelService
 * @brief Concrete implementation of the Level Service.
 *
 * Manages the timer that dictates when a boss spawns. The timer pauses
 * while the boss is active.
 */
class LevelService : public ILevelService {
   private:
    int m_CurrentLevel;
    int m_NodesDestroyedThisLevel;
    bool m_BossActive;
    bool m_LevelCompleted;
    float m_LevelTimer;
    float m_LevelDuration;

	// Refactor: No magic numbers, turned into constants
    static constexpr float MAX_PROGRESS_PERCENTAGE = 100.0f;
    static constexpr int MIN_LEVEL_INDEX = 1;

   public:
    LevelService();
    ~LevelService() override = default;

    void Initialize(int startLevel) override;

    /**
     * @brief Updates the level timer using internal boss state.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Specific update logic allowing manual boss state override.
     */
    void Update(float deltaTime, bool bossActive);
    void Reset() override;
    void Reset(int level);

    void IncrementNodesDestroyed() override;
    void SetBossActive(bool active);
    void SetLevelCompleted(bool completed);
    void StartNextLevel() override;

    int GetCurrentLevel() const override;
    int GetNodesDestroyedThisLevel() const override;
    bool IsBossActive() const;
    bool IsLevelCompleted() const override;
    bool ShouldSpawnBoss() const override;
    float GetProgressBarPercentage() const override;
};
