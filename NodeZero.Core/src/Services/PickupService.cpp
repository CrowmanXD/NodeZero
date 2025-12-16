#include "PickupService.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Config/GameConfig.h"

PickupService::PickupService()
    : m_NextPickupId(0),
    m_PickupPoints(0),
    m_ScreenHeight(0.0f) {
}

void PickupService::Initialize(float screenHeight) {
    m_ScreenHeight = screenHeight;
}

void PickupService::Update(float deltaTime) {
    for (auto& pickup : m_Pickups) {
        pickup.remainingTime -= deltaTime;
    }

    m_Pickups.erase(std::remove_if(m_Pickups.begin(), m_Pickups.end(),
        [](const PointPickup& pickup) {
            return pickup.remainingTime <= 0.0f;
        }),
        m_Pickups.end());
}

void PickupService::Reset() {
    m_Pickups.clear();
    m_NextPickupId = 0;
    m_PickupPoints = 0;
}

void PickupService::Clear() {
    Reset();
}

void PickupService::SpawnPointPickups(const Position& origin) {
    // Refactor: Random count logic kept here, but delegates to the main spawn function
    int pickupCount = 5 + (std::rand() % 6);
    SpawnPointPickups(origin, pickupCount, 1);
}

void PickupService::SpawnPointPickups(const Position& origin, int count, int pointValue) {
    // Refactor: All spawn logic consolidated here.
    for (int i = 0; i < count; ++i) {
        float angle = RandomRange(0.0f, TWO_PI);
        float radius = RandomRange(m_ScreenHeight * MIN_SPAWN_RADIUS_FACTOR,
            m_ScreenHeight * MAX_SPAWN_RADIUS_FACTOR);

        PointPickup pickup{};
        pickup.id = m_NextPickupId++;
        pickup.position.x = origin.x + std::cos(angle) * radius;
        pickup.position.y = origin.y + std::sin(angle) * radius;
        pickup.spawnOrigin = origin;
        pickup.size = m_ScreenHeight * PICKUP_SIZE_FACTOR;
        pickup.lifetime = GameConfig::PICKUP_LIFETIME;
        pickup.remainingTime = GameConfig::PICKUP_LIFETIME;
        pickup.points = pointValue;

        m_Pickups.push_back(pickup);
    }
}

bool PickupService::CollectPickup(int pickupId) {
    auto it = std::find_if(m_Pickups.begin(), m_Pickups.end(),
        [pickupId](const PointPickup& pickup) {
            return pickup.id == pickupId;
        });

    if (it == m_Pickups.end()) {
        return false;
    }

    if (it->GetAge() < GameConfig::PICKUP_COLLECT_DELAY) {
        return false;
    }

    m_PickupPoints += it->points;
    m_Pickups.erase(it);
    return true;
}

std::vector<PointPickup> PickupService::ProcessPickupCollection(float centerX, float centerY, float zoneSize) {
    std::vector<PointPickup> collected;
    ProcessPickupCollection(centerX, centerY, zoneSize, collected);
    return collected;
}

void PickupService::ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
    std::vector<PointPickup>& collectedPickups) {
    collectedPickups.clear();

    float collectRectX = mouseX - damageZoneSize / 2.0f;
    float collectRectY = mouseY - damageZoneSize / 2.0f;

    std::vector<int> idsToRemove;

    for (const PointPickup& pickup : m_Pickups) {
        if (pickup.GetAge() < GameConfig::PICKUP_COLLECT_DELAY) {
            continue;
        }

        if (CheckCollision(pickup, collectRectX, collectRectY, damageZoneSize)) {
            collectedPickups.push_back(pickup);
            idsToRemove.push_back(pickup.id);
        }
    }

    for (int id : idsToRemove) {
        CollectPickup(id);
    }
}

bool PickupService::CheckCollision(const PointPickup& pickup, float rectX, float rectY, float rectSize) const {
    bool notIntersecting = (pickup.position.x + pickup.size < rectX) ||
        (pickup.position.x - pickup.size > rectX + rectSize) ||
        (pickup.position.y + pickup.size < rectY) ||
        (pickup.position.y - pickup.size > rectY + rectSize);

    return !notIntersecting;
}

const std::vector<PointPickup>& PickupService::GetPickups() const {
    return m_Pickups;
}

int PickupService::GetPickupPoints() const {
    return m_PickupPoints;
}

float PickupService::RandomRange(float minValue, float maxValue) const {
    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minValue + (maxValue - minValue) * t;
}