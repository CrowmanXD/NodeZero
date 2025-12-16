#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

#include "../NodeZero.Core/src/Services/LevelService.h"
#include "../NodeZero.Core/src/Services/SpawnService.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Enums/NodeShape.h"
#include "../NodeZero.Core/include/Types/SpawnInfo.h"

/**
 * @class LevelServiceTest
 * @brief Tests for progression, difficulty scaling, and boss timers.
 */
class LevelServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        levelService = std::make_unique<LevelService>();
        levelService->Initialize(1);
    }
    std::unique_ptr<LevelService> levelService;
};

/** @brief Verifies level increments work. */
TEST_F(LevelServiceTest, StartNextLevelWorks) {
    levelService->StartNextLevel();
    EXPECT_EQ(levelService->GetCurrentLevel(), 2);
    // New levels should not be immediately completed
    EXPECT_FALSE(levelService->IsLevelCompleted());
}

/** @brief Verifies that the boss spawn flag triggers only after the level duration. */
TEST_F(LevelServiceTest, BossSpawningLogic) {
    EXPECT_FALSE(levelService->ShouldSpawnBoss());

    // Fast forward time
    levelService->Update(GameConfig::LEVEL_DURATION);

    EXPECT_TRUE(levelService->ShouldSpawnBoss());

    // Once active, "ShouldSpawn" should return false (already spawned)
    levelService->SetBossActive(true);
    EXPECT_FALSE(levelService->ShouldSpawnBoss());
}

/**
 * @class SpawnServiceTest
 * @brief Tests for enemy generation logic.
 */
class SpawnServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42); // Deterministic seed for testing
        spawnService = std::make_unique<SpawnService>();
        spawnService->Initialize(800.0f, 600.0f);
    }
    std::unique_ptr<SpawnService> spawnService;
};

/** @brief Verifies that spawn rate increases (interval decreases) at higher levels. */
TEST_F(SpawnServiceTest, HigherLevelsSpawnFaster) {
    // Level 1 Check
    spawnService->SetCurrentLevel(1);
    spawnService->UpdateAutoSpawn(1.5f);
    bool shouldSpawnLevel1 = spawnService->ShouldAutoSpawn();

    spawnService->ResetSpawnTimer();

    // Level 5 Check (Should be faster/more frequent)
    spawnService->SetCurrentLevel(5);
    spawnService->UpdateAutoSpawn(1.5f);
    bool shouldSpawnLevel5 = spawnService->ShouldAutoSpawn();

    // If 1.5s is not enough for Lvl 1 but is enough for Lvl 5:
    EXPECT_FALSE(shouldSpawnLevel1);
    EXPECT_TRUE(shouldSpawnLevel5);
}

/** @brief Verifies enemy HP scaling formula. */
TEST_F(SpawnServiceTest, HPScalesWithLevel) {
    float baseHP = 100.0f;

    spawnService->SetCurrentLevel(1);
    float hp1 = spawnService->CalculateNodeHP(baseHP);

    spawnService->SetCurrentLevel(5);
    float hp5 = spawnService->CalculateNodeHP(baseHP);

    EXPECT_GT(hp5, hp1);
    // Formula: Base * (1 + (Level-1)*0.2) -> 100 * (1 + 4*0.2) = 180
    EXPECT_FLOAT_EQ(hp5, baseHP * 1.8f);
}