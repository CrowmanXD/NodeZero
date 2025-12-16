#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>

#include "../NodeZero.Core/src/Services/PickupService.h"
#include "../NodeZero.Core/src/Services/DamageZoneService.h"
#include "../NodeZero.Core/src/Node.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Enums/NodeShape.h"
#include "../NodeZero.Core/include/Enums/NodeState.h"
#include "../NodeZero.Core/include/Types/PointPickup.h"
#include "../NodeZero.Core/include/Types/Position.h"

// Constants
static constexpr float TEST_SCREEN_HEIGHT = 600.0f;
static constexpr float ZONE_X = 400.0f;
static constexpr float ZONE_Y = 300.0f;

/**
 * @class PickupServiceTest
 * @brief Tests for the loot/score system.
 */
class PickupServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42);
        pickupService = std::make_unique<PickupService>();
        pickupService->Initialize(TEST_SCREEN_HEIGHT);
    }
    std::unique_ptr<PickupService> pickupService;
};

/** @brief Verifies successful collection of items after the spawn delay. */
TEST_F(PickupServiceTest, CollectPickupWorks) {
    Position origin{ ZONE_X, ZONE_Y };
    int pointsPerPickup = 2;
    pickupService->SpawnPointPickups(origin, 5, pointsPerPickup);

    // Wait for spawn animation/delay
    pickupService->Update(GameConfig::PICKUP_COLLECT_DELAY + 0.01f);

    const auto& pickups = pickupService->GetPickups();
    ASSERT_FALSE(pickups.empty());
    int pickupId = pickups[0].id;

    bool collected = pickupService->CollectPickup(pickupId);

    EXPECT_TRUE(collected);
    EXPECT_EQ(pickupService->GetPickupPoints(), pointsPerPickup);
}

/** @brief Verifies that items cannot be collected immediately after spawning. */
TEST_F(PickupServiceTest, CollectPickupFailsBeforeDelay) {
    Position origin{ ZONE_X, ZONE_Y };
    pickupService->SpawnPointPickups(origin, 5, 2);

    const auto& pickups = pickupService->GetPickups();
    int pickupId = pickups[0].id;

    // Try collecting immediately (tiny delta time)
    pickupService->Update(0.05f);

    bool collected = pickupService->CollectPickup(pickupId);
    EXPECT_FALSE(collected);
}

/**
 * @class DamageZoneServiceTest
 * @brief Tests for the player's primary attack mechanic.
 */
class DamageZoneServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        damageZoneService = std::make_unique<DamageZoneService>();
    }
    std::unique_ptr<DamageZoneService> damageZoneService;
};

/** @brief Verifies that damage is applied to nodes intersecting the zone. */
TEST_F(DamageZoneServiceTest, NodeInZoneTakesDamage) {
    auto node = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node->SetHP(100.0f);
    node->Spawn(ZONE_X, ZONE_Y); // Spawn exactly in zone center
    std::vector<Node*> nodes = { node.get() };

    float capturedCost = 0.0f;
    auto onDamaged = [&](Node* n, float cost) { capturedCost = cost; };

    // Apply damage
    damageZoneService->ProcessDamageZone(ZONE_X, ZONE_Y, 100.0f, 50.0f, 1, nodes, onDamaged);

    EXPECT_LT(node->GetHP(), 100.0f);
    EXPECT_GT(capturedCost, 0.0f); // Should return a health cost
}