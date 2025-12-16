#include "DamageZoneService.h"

#include <algorithm>
#include <cmath>

#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "Node.h"

DamageZoneService::DamageZoneService()
    : m_DamageTimer(0.0f),
      m_DamageInterval(1.5f) {
}

void DamageZoneService::UpdateTimer(float deltaTime) {
    m_DamageTimer += deltaTime;
}

void DamageZoneService::ResetTimer() {
    m_DamageTimer = 0.0f;
}

bool DamageZoneService::ShouldDealDamage() const {
    return m_DamageTimer >= m_DamageInterval;
}

void DamageZoneService::ProcessDamageZone(
    float centerX,
    float centerY,
    float zoneSize,
    float damage,
    int currentLevel,
    const std::vector<Node*>& nodes,
    std::function<void(Node*, float)> onNodeDamaged) {

    float damageRectX = centerX - zoneSize / 2.0f;
    float damageRectY = centerY - zoneSize / 2.0f;
    float damageRectRight = damageRectX + zoneSize;
    float damageRectBottom = damageRectY + zoneSize;

    for (Node* node : nodes) {
        if (node->GetState() != NodeState::Active)
            continue;

        //Refactor: Logic extracted to helper method for readability
        if (IsNodeInZone(node, damageRectX, damageRectY, damageRectRight, damageRectBottom)) {

            node->TakeDamage(damage);

            //Refactor: Cost calculation extracted to helper method
            float scaledHealthCost = CalculateDamageCost(node, currentLevel);

            if (onNodeDamaged) {
                onNodeDamaged(node, scaledHealthCost);
            }
        }
    }
}

bool DamageZoneService::IsNodeInZone(Node* node, float rectX, float rectY, float rectRight, float rectBottom) const {
    const float nodeX = node->GetPosition().x;
    const float nodeY = node->GetPosition().y;
    const float nodeSize = node->GetSize();

    float boundingRadius = nodeSize;
    const NodeShape shape = node->GetShape();

    if (shape == NodeShape::Square || shape == NodeShape::Boss) {
        boundingRadius = nodeSize * SQUARE_DIAGONAL_RATIO;
    }

    const float closestX = std::max(rectX, std::min(nodeX, rectRight));
    const float closestY = std::max(rectY, std::min(nodeY, rectBottom));

    const float deltaX = nodeX - closestX;
    const float deltaY = nodeY - closestY;
    const float distanceSquared = (deltaX * deltaX) + (deltaY * deltaY);

    return distanceSquared <= (boundingRadius * boundingRadius);
}

float DamageZoneService::CalculateDamageCost(Node* node, int currentLevel) const {
    float healthCost = BASE_HEALTH_COST;

    if (node->GetShape() == NodeShape::Boss) {
        healthCost *= BOSS_COST_MULTIPLIER;
    }

    return healthCost * (1.0f + (currentLevel - 1) * LEVEL_SCALING_FACTOR);
}