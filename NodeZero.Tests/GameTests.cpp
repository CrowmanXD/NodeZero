#include <gtest/gtest.h>
#include <memory>

#include "../NodeZero.Core/src/Game.h"
#include "../NodeZero.Core/include/IGame.h"
#include "../NodeZero.Core/include/INode.h"
#include "../NodeZero.Core/include/Events/IObserver.h"
#include "../NodeZero.Core/include/Events/IEvent.h"
#include "../NodeZero.Core/include/Types/SpawnInfo.h"

// Constants
static constexpr float TEST_WIDTH = 800.0f;
static constexpr float TEST_HEIGHT = 600.0f;
static constexpr float TEST_DELTA_TIME = 0.016f;
static constexpr int STRESS_TEST_COUNT = 50;

static SpawnInfo CreateTestSpawnInfo(float x, float y) {
    SpawnInfo info;
    info.position = Position{ x, y };
    info.shape = NodeShape::Circle;
    info.directionX = 0.0f;
    info.directionY = 0.0f;
    return info;
}

/**
 * @class MockObserver
 * @brief Simple observer implementation to verify event broadcasting.
 */
class MockObserver : public IObserver {
private:
    int m_EventCount = 0;
public:
    void Update(const std::shared_ptr<IEvent>& event) override {
        m_EventCount++;
    }
    int GetEventCount() const { return m_EventCount; }
};

/**
 * @class GameTest
 * @brief Integration tests for the main Game class.
 */
class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(TEST_WIDTH, TEST_HEIGHT);
        game->Reset();
    }
    std::unique_ptr<IGame> game;
};

/** @brief Verifies the game starts in a valid default state. */
TEST_F(GameTest, InitializeWorks) {
    EXPECT_FLOAT_EQ(game->GetScreenWidth(), TEST_WIDTH);
    EXPECT_FLOAT_EQ(game->GetScreenHeight(), TEST_HEIGHT);
    EXPECT_EQ(game->GetNodes().size(), 0);
    EXPECT_FALSE(game->GetHealthService().IsZero());
}

/** @brief Verifies the game loop advances time and systems without crashing. */
TEST_F(GameTest, UpdateDoesNotCrash) {
    EXPECT_NO_THROW(game->Update(TEST_DELTA_TIME));
}

/** @brief Verifies that Update() eventually causes enemies to spawn via the SpawnService. */
TEST_F(GameTest, UpdateSpawnsNodesOverTime) {
    size_t initialNodeCount = game->GetNodes().size();

    // Run enough frames to trigger a spawn
    for (int i = 0; i < STRESS_TEST_COUNT; ++i) {
        game->Update(0.1f);
    }

    EXPECT_GT(game->GetNodes().size(), initialNodeCount);
}

/** @brief Verifies that Reset() removes all active entities. */
TEST_F(GameTest, ResetClearsNodes) {
    game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
    game->Update(1.0f); // Allow one frame of processing

    game->Reset();

    EXPECT_EQ(game->GetNodes().size(), 0);
    EXPECT_EQ(game->GetNodesDestroyed(), 0);
}

/** @brief Verifies that the Observer pattern is wired up correctly. */
TEST_F(GameTest, ObserverPatternWorks) {
    auto observer = std::make_shared<MockObserver>();

    game->Attach(observer);
    EXPECT_EQ(observer->GetEventCount(), 0);

    // Simulate activity that should trigger events (spawning)
    for (int i = 0; i < 50; ++i) {
        game->Update(0.1f);
    }

    EXPECT_GT(observer->GetEventCount(), 0);
    game->Detach(observer);
}

/** @brief Stress test: Ensure spawning many nodes doesn't segfault. */
TEST_F(GameTest, ManyNodesDoNotCrash) {
    for (int i = 0; i < STRESS_TEST_COUNT; ++i) {
        game->SpawnNode(CreateTestSpawnInfo(400.0f + i * 10, 300.0f));
    }

    EXPECT_NO_THROW(game->Update(TEST_DELTA_TIME));
}