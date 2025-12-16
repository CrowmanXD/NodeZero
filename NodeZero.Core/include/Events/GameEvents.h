#pragma once

#include <string>

#include "Enums/EventType.h"
#include "Enums/GameScreen.h"
#include "Enums/NodeShape.h"
#include "IEvent.h"
#include "Types/Position.h"

/**
 * @class GameEvent
 * @brief A comprehensive event structure holding data for game logic events.
 *
 * This "God Class" approach allows a single type to handle data for
 * nodes, scoring, game states, and level progression.
 */
class GameEvent : public IEvent {
public:
    // --- Header Data ---
    EventType type;
    float timestamp;

    // --- Node/Entity Data ---
    NodeShape shape = NodeShape::Circle;
    Position position = { 0.0f, 0.0f };
    float size = 0.0f;
    int hp = 0;
    int damage = 0;

    // --- Scoring Data ---
    int points = 0;
    int delta = 0;             // Change in value (points added/removed)
    float multiplier = 1.0f;
    float oldMultiplier = 1.0f;

    // --- Game State/Level Data ---
    GameScreen screenState = GameScreen::MainMenu;
    GameScreen oldScreenState = GameScreen::MainMenu;
    int level = 1;
    int nextLevel = 1;
    float bossHP = 0.0f;

    // -------------------------------------------------------------------------
    // Constructors (Refactor: Logic Simplification)
    // -------------------------------------------------------------------------

    /** @brief Default/Generic Constructor */
    GameEvent(float timestamp, EventType type)
        : type(type), timestamp(timestamp) {
    }

    /** @brief Constructor for Node-related events (Spawn, Damage, etc.) */
    GameEvent(float timestamp, EventType type, NodeShape shape, Position pos, float size)
        : type(type), timestamp(timestamp), shape(shape), position(pos), size(size) {
    }

    /** @brief Constructor for Score/Multiplier events */
    GameEvent(float timestamp, EventType type, int points, float multiplier)
        : type(type), timestamp(timestamp), points(points), multiplier(multiplier) {
    }

    // -------------------------------------------------------------------------
    // Interface Implementation
    // -------------------------------------------------------------------------

    float GetTimestamp() const override {
        return timestamp;
    }

    std::string GetType() const override {
        switch (type) {
        case EventType::NodeSpawned:      return "NodeSpawned";
        case EventType::NodeDamaged:      return "NodeDamaged";
        case EventType::NodeDestroyed:    return "NodeDestroyed";
        case EventType::PointsChanged:    return "PointsChanged";
        case EventType::MultiplierChanged:return "MultiplierChanged";
        case EventType::GameStateChanged: return "GameStateChanged";
        case EventType::GameOver:         return "GameOver";
        case EventType::BossSpawned:      return "BossSpawned";
        case EventType::BossDefeated:     return "BossDefeated";
        case EventType::LevelCompleted:   return "LevelCompleted";
        default:                          return "Unknown";
        }
    }
};