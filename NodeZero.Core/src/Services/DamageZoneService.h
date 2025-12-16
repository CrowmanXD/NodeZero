#pragma once

#include <functional>
#include <vector>

#include "Services/IDamageZoneService.h"

class Node;
enum class NodeShape;

/**
 * @class DamageZoneService
 * @brief Concrete implementation of the damage zone logic.
 * * Handles the logic for Rectangle-vs-Circle collision detection
 * and damage scaling calculations.
 */
class DamageZoneService : public IDamageZoneService {
   private:
    float m_DamageTimer;
    float m_DamageInterval;

	//Refactor: No magic numbers, turned into constants
    static constexpr float SQUARE_DIAGONAL_RATIO = 1.414f; // approx sqrt(2)
    static constexpr float BASE_HEALTH_COST = 0.5f;
    static constexpr float BOSS_COST_MULTIPLIER = 8.0f;
    static constexpr float LEVEL_SCALING_FACTOR = 0.20f;

   public:
    DamageZoneService();
    ~DamageZoneService() override = default;

    void UpdateTimer(float deltaTime) override;
    void ResetTimer() override;
    bool ShouldDealDamage() const override;

    void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<Node*>& nodes,
        std::function<void(Node*, float)> onNodeDamaged) override;

    private:
    /**
     * @brief Checks if a specific node is colliding with the damage rect.
     */
    bool IsNodeInZone(Node* node, float rectX, float rectY, float rectRight, float rectBottom) const;

    /**
     * @brief Calculates the resource cost/reward for damaging a node.
     */
    float CalculateDamageCost(Node* node, int currentLevel) const;
};
