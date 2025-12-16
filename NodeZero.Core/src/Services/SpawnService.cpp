#include "SpawnService.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Config/GameConfig.h"

SpawnService::SpawnService()
    : m_ScreenWidth(0.0f),
    m_ScreenHeight(0.0f),
    m_SpawnTimer(0.0f),
    m_CurrentLevel(1) {
}

void SpawnService::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void SpawnService::UpdateAutoSpawn(float deltaTime) {
    m_SpawnTimer += deltaTime;
}

void SpawnService::ResetSpawnTimer() {
    m_SpawnTimer = 0.0f;
}

void SpawnService::SetCurrentLevel(int level) {
    m_CurrentLevel = level;
}

SpawnInfo SpawnService::GetNextSpawn() const {
    const float centerX = m_ScreenWidth / 2.0f;
    const float centerY = m_ScreenHeight / 2.0f;

    int edge = std::rand() % 4;
    float spawnX = 0.0f;
    float spawnY = 0.0f;

    // Refactor: Use constants for the offset
    switch (edge) {
    case 0:
        spawnX = RandomRange(SPAWN_EDGE_OFFSET, m_ScreenWidth - SPAWN_EDGE_OFFSET);
        spawnY = -SPAWN_EDGE_OFFSET;
        break;
    case 1:
        spawnX = m_ScreenWidth + SPAWN_EDGE_OFFSET;
        spawnY = RandomRange(SPAWN_EDGE_OFFSET, m_ScreenHeight - SPAWN_EDGE_OFFSET);
        break;
    case 2:
        spawnX = RandomRange(SPAWN_EDGE_OFFSET, m_ScreenWidth - SPAWN_EDGE_OFFSET);
        spawnY = m_ScreenHeight + SPAWN_EDGE_OFFSET;
        break;
    case 3:
        spawnX = -SPAWN_EDGE_OFFSET;
        spawnY = RandomRange(SPAWN_EDGE_OFFSET, m_ScreenHeight - SPAWN_EDGE_OFFSET);
        break;
    default:
        spawnX = centerX;
        spawnY = -SPAWN_EDGE_OFFSET;
        break;
    }

    float targetX = centerX + RandomRange(-TARGET_CENTER_VARIANCE, TARGET_CENTER_VARIANCE);
    float targetY = centerY + RandomRange(-TARGET_CENTER_VARIANCE, TARGET_CENTER_VARIANCE);

    float dirX = targetX - spawnX;
    float dirY = targetY - spawnY;

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;
    }

    return SpawnInfo{
        Position{spawnX, spawnY},
        GetRandomShape(),
        dirX,
        dirY
    };
}

NodeShape SpawnService::GetRandomShape() const {
    int chance = std::rand() % 100;

    // Refactor: Use named probability constants
    if (chance < CHANCE_SQUARE) {
        return NodeShape::Square;
    }
    else if (chance < CHANCE_CIRCLE) {
        return NodeShape::Circle;
    }
    else {
        return NodeShape::Hexagon;
    }
}

float SpawnService::CalculateNodeHP(float baseHP) const {
    return baseHP * (1.0f + (m_CurrentLevel - 1) * HP_SCALING_FACTOR);
}

float SpawnService::RandomRange(float minValue, float maxValue) const {
    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minValue + (maxValue - minValue) * t;
}

bool SpawnService::ShouldAutoSpawn() const {
    float calculatedInterval = BASE_SPAWN_INTERVAL - (m_CurrentLevel - 1) * INTERVAL_DECAY_RATE;

    // Refactor: Ensure we never go below the minimum limit
    float finalInterval = std::max(MIN_SPAWN_INTERVAL, calculatedInterval);

    return m_SpawnTimer >= finalInterval;
}