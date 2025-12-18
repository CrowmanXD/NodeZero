/**
 * @file ServiceTests.cpp
 * @brief Unit tests for Health, Upgrade, and Save services.
 */
#include <gtest/gtest.h>

#include "../NodeZero.Core/src/Services/HealthService.h"
#include "../NodeZero.Core/src/Services/UpgradeService.h"
#include "../NodeZero.Core/src/Services/SaveService.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Types/SaveData.h"

/**
 * @class HealthServiceTest
 * @brief Tests for player health, regeneration, and depletion.
 */
class HealthServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        healthService = std::make_unique<HealthService>();
        healthService->Initialize(100.0f, 5.0f);
    }
    std::unique_ptr<HealthService> healthService;
};

/** @brief Verifies passive regeneration logic over time. */
TEST_F(HealthServiceTest, RegenerationWorks) {
    healthService->Reduce(50.0f); // Current = 50
    healthService->Update(2.0f);  // +5/sec * 2sec = +10

    EXPECT_NEAR(healthService->GetCurrent(), 60.0f, 0.1f);
}

/** @brief Verifies that health decays faster at higher levels. */
TEST_F(HealthServiceTest, DepletionScalesWithLevel) {
    // Check Level 1 decay
    healthService->SetCurrentLevel(1);
    healthService->RestoreToMax();
    healthService->Update(0.3f); // Trigger depletion tick
    float level1Health = healthService->GetCurrent();

    // Check Level 5 decay
    healthService->SetCurrentLevel(5);
    healthService->RestoreToMax();
    healthService->Update(0.3f);
    float level5Health = healthService->GetCurrent();

    // Higher level -> More depletion -> Lower health remaining
    EXPECT_LT(level5Health, level1Health);
}

/**
 * @class MockSaveService
 * @brief Mock implementation to isolate UpgradeService tests from disk I/O.
 */
class MockSaveService : public ISaveService {
private:
    SaveData m_Data;
public:
    MockSaveService() {
        m_Data.points = 1000; // Start with plenty of cash
        m_Data.maxHealth = GameConfig::HEALTH_DEFAULT;
        m_Data.regenRate = 0.0f;
        m_Data.damageZoneSize = GameConfig::DAMAGE_ZONE_DEFAULT_SIZE;
        m_Data.damagePerTick = GameConfig::DAMAGE_PER_TICK_DEFAULT;
    }

    SaveData LoadProgress() override { return m_Data; }
    void SaveProgress(const SaveData& data) override { m_Data = data; }
    int GetPoints() const override { return m_Data.points; }
    int GetHighPoints() const override { return m_Data.highPoints; }
    SaveData GetCurrentData() const override { return m_Data; }
};

/**
 * @class UpgradeServiceTest
 * @brief Tests for the shop/purchase logic.
 */
class UpgradeServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        upgradeService = std::make_unique<UpgradeService>();
        mockSaveService = std::make_unique<MockSaveService>();
        upgradeService->SetSaveService(mockSaveService.get());
        upgradeService->Initialize(GameConfig::HEALTH_DEFAULT, 0.0f, GameConfig::DAMAGE_ZONE_DEFAULT_SIZE, GameConfig::DAMAGE_PER_TICK_DEFAULT);
    }
    std::unique_ptr<UpgradeService> upgradeService;
    std::unique_ptr<MockSaveService> mockSaveService;
};

/** @brief Verifies that purchasing an upgrade deducts points and applies stats. */
TEST_F(UpgradeServiceTest, BuyHealthUpgradeWorks) {
    int initialPoints = mockSaveService->GetPoints();
    float initialHealth = upgradeService->GetMaxHealth();

    EXPECT_TRUE(upgradeService->BuyHealthUpgrade());

    // Check Stat Increase
    EXPECT_FLOAT_EQ(upgradeService->GetMaxHealth(), initialHealth + 1.0f);
    // Check Cost Deduction
    EXPECT_EQ(mockSaveService->GetPoints(), initialPoints - GameConfig::HEALTH_UPGRADE_COST);
}

/** @brief Verifies that upgrades fail gracefully when the player is poor. */
TEST_F(UpgradeServiceTest, UpgradesFailWithInsufficientPoints) {
    // Bankrupt the player
    SaveData data = mockSaveService->GetCurrentData();
    data.points = 10;
    mockSaveService->SaveProgress(data);

    EXPECT_FALSE(upgradeService->BuyHealthUpgrade());
    EXPECT_FALSE(upgradeService->BuyRegenUpgrade());
}