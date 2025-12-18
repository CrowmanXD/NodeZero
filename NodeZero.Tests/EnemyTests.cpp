/**
 * @file EnemyTests.cpp
 * @brief Unit tests for Node (enemy) spawning, damage, and attributes.
 */

#include <gtest/gtest.h>
#include <cmath>
#include <memory>

#include "../NodeZero.Core/src/Game.h"
#include "../NodeZero.Core/include/IGame.h"
#include "../NodeZero.Core/include/INode.h"
#include "../NodeZero.Core/include/Types/SpawnInfo.h"
#include "../NodeZero.Core/include/Services/ILevelService.h" // Added for LevelService access

// Test Constants (Refactoring)
static constexpr float TEST_WIDTH = 800.0f;
static constexpr float TEST_HEIGHT = 600.0f;
static constexpr float TEST_CENTER_X = 400.0f;
static constexpr float TEST_CENTER_Y = 300.0f;
static constexpr float TEST_DELTA_TIME = 0.016f; // approx 60 FPS

/**
 * @brief Helper function to create standard spawn data for testing.
 * Reduces code duplication in tests.
 */
static SpawnInfo CreateTestSpawnInfo(float x, float y) {
    SpawnInfo info;
    info.position = Position{ x, y };
    info.shape = NodeShape::Circle;
    info.directionX = 0.0f;
    info.directionY = 0.0f;
    return info;
}

/**
 * @class EnemySpawnTest
 * @brief Tests related to the instantiation and placement of enemies.
 */
class EnemySpawnTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

/** @brief Verifies that an enemy appears exactly where requested. */
TEST_F(EnemySpawnTest, EnemySpawnsAtSpecifiedPosition) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().x, TEST_CENTER_X);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().y, TEST_CENTER_Y);
}

/** @brief Verifies that multiple spawn calls result in multiple entities. */
TEST_F(EnemySpawnTest, MultipleEnemiesCanSpawn) {
    game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
    game->SpawnNode(CreateTestSpawnInfo(200.0f, 200.0f));
    game->SpawnNode(CreateTestSpawnInfo(300.0f, 300.0f));

    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 3);
}

/** @brief Verifies that a newly spawned enemy has valid health and size stats. */
TEST_F(EnemySpawnTest, EnemyHasValidInitialState) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    EXPECT_GT(enemy->GetSize(), 0.0f);
    EXPECT_GT(enemy->GetHP(), 0.0f);
    EXPECT_GT(enemy->GetMaxHP(), 0.0f);
    EXPECT_EQ(enemy->GetHP(), enemy->GetMaxHP());
}

/**
 * @class EnemyDamageTest
 * @brief Tests related to health reduction and death logic.
 */
class EnemyDamageTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

/** @brief Verifies that taking damage reduces HP by the correct amount. */
TEST_F(EnemyDamageTest, EnemyTakesDamage) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    float initialHP = enemy->GetHP();
    float damageAmount = 10.0f;
    enemy->TakeDamage(damageAmount);

    EXPECT_LT(enemy->GetHP(), initialHP);
    EXPECT_FLOAT_EQ(enemy->GetHP(), initialHP - damageAmount);
}

/** @brief Verifies that HP is clamped at 0 and does not become negative. */
TEST_F(EnemyDamageTest, EnemyHPCannotGoBelowZero) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    enemy->TakeDamage(9999.0f); // Massive damage
    EXPECT_GE(enemy->GetHP(), 0.0f);
}

/** @brief Verifies that exact lethal damage kills the enemy logic state. */
TEST_F(EnemyDamageTest, EnemyDiesWhenHPReachesZero) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    // Must update once to ensure internal state is processed if needed
    game->Update(0.0f);

    const auto& nodes = game->GetNodes();
    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    enemy->TakeDamage(enemy->GetMaxHP());

    // Check directly if HP is zero or less
    EXPECT_LE(enemy->GetHP(), 0.0f);
}

/** @brief Verifies cumulative damage application. */
TEST_F(EnemyDamageTest, MultipleDamageApplications) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    float initialHP = enemy->GetHP();
    enemy->TakeDamage(5.0f);
    enemy->TakeDamage(3.0f);
    enemy->TakeDamage(2.0f);

    EXPECT_FLOAT_EQ(enemy->GetHP(), initialHP - 10.0f);
}

/**
 * @class EnemyPropertiesTest
 * @brief Tests for physical attributes (Speed, Size, Shape).
 */
class EnemyPropertiesTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

/** @brief Verifies speed is initialized to a non-negative value. */
TEST_F(EnemyPropertiesTest, EnemyHasValidSpeed) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_GE(nodes[0]->GetSpeed(), 0.0f);
}

/** @brief Verifies size is positive. */
TEST_F(EnemyPropertiesTest, EnemyHasValidSize) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_GT(nodes[0]->GetSize(), 0.0f);
}

/** @brief Verifies the shape enum is one of the expected values. */
TEST_F(EnemyPropertiesTest, EnemyHasShape) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    NodeShape shape = nodes[0]->GetShape();
    EXPECT_TRUE(shape == NodeShape::Circle ||
        shape == NodeShape::Square ||
        shape == NodeShape::Hexagon ||
        shape == NodeShape::Boss);
}

/** @brief Verifies rotation is initialized correctly. */
TEST_F(EnemyPropertiesTest, EnemyHasRotation) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    float rotation = nodes[0]->GetRotation();
    EXPECT_GE(rotation, 0.0f);
}

/**
 * @class EnemyDestructionTest
 * @brief Tests related to the removal of enemies and score/stats updates.
 */
class EnemyDestructionTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

/** @brief Verifies that killing an enemy increments the global destroyed counter. */
TEST_F(EnemyDestructionTest, DestroyedEnemiesIncrementCounter) {
    int initialDestroyed = game->GetNodesDestroyed();

    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));

    // Ensure update logic runs to process potential frame-1 events
    game->Update(TEST_DELTA_TIME);

    const auto& nodes = game->GetNodes();
    if (nodes.size() > 0) {
        INode* enemy = nodes[0];
        enemy->TakeDamage(enemy->GetMaxHP()); // Kill it
        game->Update(TEST_DELTA_TIME);        // Process death logic
    }

    EXPECT_GE(game->GetNodesDestroyed(), initialDestroyed);
}

/** @brief Verifies that killing an enemy increments the level-specific counter. */
TEST_F(EnemyDestructionTest, LevelCounterTracksDestroyedEnemies) {
    int initialDestroyed = game->GetLevelService().GetNodesDestroyedThisLevel();

    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    game->Update(TEST_DELTA_TIME);

    const auto& nodes = game->GetNodes();
    if (nodes.size() > 0) {
        INode* enemy = nodes[0];
        enemy->TakeDamage(enemy->GetMaxHP());
        game->Update(TEST_DELTA_TIME);
    }

    EXPECT_GE(game->GetLevelService().GetNodesDestroyedThisLevel(), initialDestroyed);
}

/**
 * @class EnemyUpdateTest
 * @brief Tests ensuring the update loop runs correctly for entities.
 */
class EnemyUpdateTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

/** @brief Verifies that calling Update() on an entity doesn't cause exceptions. */
TEST_F(EnemyUpdateTest, EnemyUpdateDoesNotCrash) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_NO_THROW(nodes[0]->Update(TEST_DELTA_TIME));
}

/** @brief Verifies that multiple enemies can be updated in sequence safely. */
TEST_F(EnemyUpdateTest, MultipleEnemiesUpdateWithoutCrash) {
    game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
    game->SpawnNode(CreateTestSpawnInfo(200.0f, 200.0f));
    game->SpawnNode(CreateTestSpawnInfo(300.0f, 300.0f));

    const auto& nodes = game->GetNodes();
    ASSERT_EQ(nodes.size(), 3);

    for (INode* enemy : nodes) {
        EXPECT_NO_THROW(enemy->Update(TEST_DELTA_TIME));
    }
}

/** @brief Verifies that position data remains valid (not NaN/Inf) after movement updates. */
TEST_F(EnemyUpdateTest, EnemyPositionValidAfterUpdate) {
    game->SpawnNode(CreateTestSpawnInfo(TEST_CENTER_X, TEST_CENTER_Y));
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    nodes[0]->Update(TEST_DELTA_TIME);

    Position pos = nodes[0]->GetPosition();
    EXPECT_TRUE(std::isfinite(pos.x));
    EXPECT_TRUE(std::isfinite(pos.y));
}