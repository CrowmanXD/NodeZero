#pragma once

#include <vector>

#include "Services/IPickupService.h"
#include "Types/PointPickup.h"
#include "Types/Position.h"

/**
 * @class PickupService
 * @brief Concrete implementation of the pickup system.
 *
 * Manages the lifecycle, random spawning logic, and collision detection
 * for point pickups.
 */
class PickupService : public IPickupService {
   private:
    std::vector<PointPickup> m_Pickups;
    int m_NextPickupId;
    int m_PickupPoints;
    float m_ScreenHeight;

	//Refactor: No magic numbers, turned into constants
    static constexpr float TWO_PI = 6.28318530718f;
    static constexpr float MIN_SPAWN_RADIUS_FACTOR = 0.0125f;
    static constexpr float MAX_SPAWN_RADIUS_FACTOR = 0.05f;
    static constexpr float PICKUP_SIZE_FACTOR = 0.0075f;

   public:
    PickupService();
    ~PickupService() override = default;

    /**
     * @brief Initializes the service with screen dimensions for scaling.
     */
    void Initialize(float screenHeight);
    void Update(float deltaTime) override;
    void Clear() override;
    void Reset();

    void SpawnPointPickups(const Position& origin, int count, int pointValue) override;
    void SpawnPointPickups(const Position& origin);
    bool CollectPickup(int pickupId) override;

    /**
     * @brief Deprecated/Legacy method. Consider using the void version instead.
     */
    std::vector<PointPickup> ProcessPickupCollection(float centerX, float centerY, float zoneSize);
    void ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
                                 std::vector<PointPickup>& collectedPickups) override;

    const std::vector<PointPickup>& GetPickups() const override;
    int GetPickupPoints() const override;

   private:
    float RandomRange(float minValue, float maxValue) const;

    /**
     * @brief Helper to check AABB (Axis-Aligned Bounding Box) collision.
     */
    bool CheckCollision(const PointPickup& pickup, float rectX, float rectY, float rectSize) const;
};
