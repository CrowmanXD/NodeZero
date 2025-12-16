#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "Events/GameEvents.h"
#include "Events/IObserver.h"

/**
 * @class EventLogger
 * @brief Observer that listens to game events and logs them to the console.
 *
 * Useful for debugging logic flows or tracking player actions without
 * setting breakpoints.
 */
class EventLogger : public IObserver {
public:
    EventLogger() = default;
    virtual ~EventLogger() = default;

    void Update(const std::shared_ptr<IEvent>& event) override {
        if (!event) return;

        auto gameEvent = std::static_pointer_cast<GameEvent>(event);

        switch (gameEvent->type) {
        case EventType::NodeSpawned:      LogNodeSpawn(gameEvent); break;
        case EventType::NodeDestroyed:    LogNodeDestroy(gameEvent); break;
        case EventType::NodeDamaged:      LogNodeDamage(gameEvent); break;
        case EventType::BossSpawned:      LogBossSpawn(gameEvent); break;
        case EventType::BossDefeated:     LogBossDefeat(gameEvent); break;
        case EventType::LevelCompleted:   LogLevelComplete(gameEvent); break;
        default: break;
        }
    }

private:
    // Helper to standardize output format
    void Log(const std::string& msg) {
        std::cout << "[GAME EVENT] " << msg << std::endl;
    }

    void LogNodeSpawn(const std::shared_ptr<GameEvent>& e) {
        Log("Spawn: Node at (" + std::to_string(e->position.x) + ", " + std::to_string(e->position.y) + ")");
    }

    void LogNodeDestroy(const std::shared_ptr<GameEvent>& e) {
        Log("Destroy: Points gained: " + std::to_string(e->points));
    }

    void LogNodeDamage(const std::shared_ptr<GameEvent>& e) {
        Log("Damage: " + std::to_string(e->damage) + " dealt. Remaining HP: " + std::to_string(e->hp));
    }

    void LogBossSpawn(const std::shared_ptr<GameEvent>& e) {
        Log("BOSS SPAWN: Level " + std::to_string(e->level) + ", HP: " + std::to_string(e->bossHP));
    }

    void LogBossDefeat(const std::shared_ptr<GameEvent>& e) {
        Log("BOSS DEFEATED: Level " + std::to_string(e->level));
    }

    void LogLevelComplete(const std::shared_ptr<GameEvent>& e) {
        Log("LEVEL COMPLETE: Advancing to Level " + std::to_string(e->nextLevel));
    }
};