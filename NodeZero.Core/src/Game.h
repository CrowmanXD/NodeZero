#pragma once

#include <memory>
#include <vector>

#include "Events/Subject.h"
#include "IGame.h"
#include "Node.h"
#include "Services/DamageZoneService.h"
#include "Services/HealthService.h"
#include "Services/LevelService.h"
#include "Services/PickupService.h"
#include "Services/SaveService.h"
#include "Services/SpawnService.h"
#include "Services/UpgradeService.h"
#include "Types/PointPickup.h"

/**
 * @class Game
 * @brief The Core Game Loop implementation.
 *
 * Orchestrates the update order of services, manages the entity list (nodes),
 * and handles collision/interaction logic.
 */
class Game : public IGame {
private:
    Subject m_Subject;

    // Refactor: Store INode* to avoid unsafe reinterpret_cast in getter
    std::vector<INode*> m_Nodes;

    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_ElapsedTime;

    int m_NodesDestroyed;
    int m_HighPoints;

    INode* m_Boss; // Stored as generic interface

    float m_MouseX;
    float m_MouseY;
    std::vector<PointPickup> m_CollectedPickupsThisFrame;

    UpgradeService m_UpgradeService;
    PickupService m_PickupService;
    HealthService m_HealthService;
    SpawnService m_SpawnService;
    LevelService m_LevelService;
    DamageZoneService m_DamageZoneService;
    SaveService m_SaveService;

	// Refactor: No magic numbers, turned into constants
    static constexpr float BOSS_OFFSCREEN_LIMIT = -200.0f;
    static constexpr int POINTS_BOSS = 500;
    static constexpr int POINTS_NODE = 100;

public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;
    void Reset() override;
    void StartNextLevel() override;

    void SetMousePosition(float x, float y) override;
    void SpawnNode(const SpawnInfo& info) override;

    float GetScreenWidth() const override;
    float GetScreenHeight() const override;

    const std::vector<INode*>& GetNodes() const override;
    std::vector<PointPickup> GetCollectedPickupsThisFrame() const override;

    int GetNodesDestroyed() const override;
    void SaveProgress() override;
    int GetHighPoints() const override;

    // Service Accessors
    IUpgradeService& GetUpgradeService() override;
    IPickupService& GetPickupService() override;
    IHealthService& GetHealthService() override;
    ILevelService& GetLevelService() override;
    IDamageZoneService& GetDamageZoneService() override;
    ISpawnService& GetSpawnService() override;
    ISaveService& GetSaveService() override;

    // Observer Pattern
    void Attach(std::shared_ptr<IObserver> observer) override;
    void Detach(std::shared_ptr<IObserver> observer) override;
    void Notify(const std::shared_ptr<IEvent>& event) override;

private:
    INode* CreateNode(NodeShape shape, float size, float speed);
    void SpawnBoss();

    // Refactor: Breaking down Update loop
    void HandleSpawning(float deltaTime);
    void HandleDamageZones(float deltaTime);
    void UpdateNodes(float deltaTime);
};