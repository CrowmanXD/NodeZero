#include "LevelService.h"

#include "Config/GameConfig.h"

LevelService::LevelService()
    : m_CurrentLevel(MIN_LEVEL_INDEX),
      m_NodesDestroyedThisLevel(0),
      m_BossActive(false),
      m_LevelCompleted(false),
      m_LevelTimer(0.0f),
      m_LevelDuration(GameConfig::LEVEL_DURATION) {
}

void LevelService::Initialize(int startLevel) {
    m_CurrentLevel = startLevel;
    if (m_CurrentLevel < MIN_LEVEL_INDEX) {
        m_CurrentLevel = MIN_LEVEL_INDEX;
    }
}

void LevelService::Update(float deltaTime) {
    // This function satisfies the ILevelService interface.
    // It forwards the call to the internal logic using the class's own boss state.
    Update(deltaTime, m_BossActive);
}

void LevelService::Update(float deltaTime, bool bossActive) {
    // Refactor: Logic is cleaner. Timer stops when boss is fighting.
    // Also ensures we don't process negative time steps.
    if (!bossActive && deltaTime > 0.0f) {
        m_LevelTimer += deltaTime;
    }
}

void LevelService::Reset() {
    // Refactor: No duplicate code; just call the parameterized version.
    Reset(MIN_LEVEL_INDEX);
}

void LevelService::Reset(int level) {
    m_CurrentLevel = level;
    if (m_CurrentLevel < MIN_LEVEL_INDEX) {
        m_CurrentLevel = MIN_LEVEL_INDEX;
    }

    m_NodesDestroyedThisLevel = 0;
    m_BossActive = false;
    m_LevelCompleted = false;
    m_LevelTimer = 0.0f;
}

void LevelService::IncrementNodesDestroyed() {
    m_NodesDestroyedThisLevel++;
}

void LevelService::SetBossActive(bool active) {
    m_BossActive = active;
}

void LevelService::SetLevelCompleted(bool completed) {
    m_LevelCompleted = completed;
}

void LevelService::StartNextLevel() {
    m_CurrentLevel++;
    m_NodesDestroyedThisLevel = 0;
    m_LevelCompleted = false;
    m_LevelTimer = 0.0f;
    m_BossActive = false;
}

int LevelService::GetCurrentLevel() const {
    return m_CurrentLevel;
}

int LevelService::GetNodesDestroyedThisLevel() const {
    return m_NodesDestroyedThisLevel;
}

bool LevelService::IsBossActive() const {
    return m_BossActive;
}

bool LevelService::IsLevelCompleted() const {
    return m_LevelCompleted;
}

bool LevelService::ShouldSpawnBoss() const {
    return m_LevelTimer >= m_LevelDuration && !m_BossActive;
}

float LevelService::GetProgressBarPercentage() const {
    // Refactor: Prevent division by zero
    if (m_LevelDuration <= 0.0f) return MAX_PROGRESS_PERCENTAGE;

    float percentage = (m_LevelTimer / m_LevelDuration) * MAX_PROGRESS_PERCENTAGE;

    if (percentage > MAX_PROGRESS_PERCENTAGE) {
        percentage = MAX_PROGRESS_PERCENTAGE;
    }
    return percentage;
}
