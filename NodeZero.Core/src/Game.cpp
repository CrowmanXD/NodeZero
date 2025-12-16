#include "Game.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Config/GameConfig.h"
#include "Events/GameEvents.h"

Game::Game()
    : m_ScreenWidth(0.0f),
    m_ScreenHeight(0.0f),
    m_ElapsedTime(0.0f),
    m_NodesDestroyed(0),
    m_HighPoints(0),
    m_Boss(nullptr),
    m_MouseX(0.0f),
    m_MouseY(0.0f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SaveData saveData = m_SaveService.LoadProgress();
    m_HighPoints = saveData.highPoints;

    m_UpgradeService.SetSaveService(&m_SaveService);
    m_UpgradeService.Initialize(saveData.maxHealth, saveData.regenRate, saveData.damageZoneSize, saveData.damagePerTick);
    m_HealthService.Initialize(saveData.maxHealth, saveData.regenRate);
    m_LevelService.Initialize(saveData.currentLevel);
}

Game::~Game() {
    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();
}

void Game::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;

    m_PickupService.Initialize(screenHeight);
    m_SpawnService.Initialize(screenWidth, screenHeight);
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());
}

// -----------------------------------------------------------------------------
// Core Loop (Refactored)
// -----------------------------------------------------------------------------

void Game::Update(float deltaTime) {
    m_CollectedPickupsThisFrame.clear();

    // 1. Update Services
    m_HealthService.Update(deltaTime);
    m_HealthService.SetCurrentLevel(m_LevelService.GetCurrentLevel());
    m_LevelService.Update(deltaTime, m_LevelService.IsBossActive());

    // 2. Logic Steps extracted to helpers
    HandleSpawning(deltaTime);
    HandleDamageZones(deltaTime);

    // 3. Pickups
    m_CollectedPickupsThisFrame = m_PickupService.ProcessPickupCollection(
        m_MouseX, m_MouseY, m_UpgradeService.GetDamageZoneSize()
    );
    m_PickupService.Update(deltaTime);

    // 4. Update Entities and check for deaths
    UpdateNodes(deltaTime);

    m_ElapsedTime += deltaTime;
}

// -----------------------------------------------------------------------------
// Helper Logic
// -----------------------------------------------------------------------------

void Game::HandleSpawning(float deltaTime) {
    m_SpawnService.UpdateAutoSpawn(deltaTime);
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());

    if (m_SpawnService.ShouldAutoSpawn()) {
        SpawnInfo info = m_SpawnService.GetNextSpawn();
        SpawnNode(info);
        m_SpawnService.ResetSpawnTimer();
    }

    if (m_LevelService.ShouldSpawnBoss()) {
        SpawnBoss();
    }
}

void Game::HandleDamageZones(float deltaTime) {
    m_DamageZoneService.UpdateTimer(deltaTime);

    if (m_DamageZoneService.ShouldDealDamage()) {
        m_DamageZoneService.ResetTimer();

        // Lambda to handle what happens when a specific node gets hit
        auto onNodeDamaged = [this](INode* node, float healthCost) { // generic INode*
            // Refactor: Using INode interface methods
            if (Node* concreteNode = dynamic_cast<Node*>(node)) { // safety cast if needed, though INode suffices for most
                // Events usually expect Position
            }

            auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeDamaged);
            event->position = node->GetPosition();
            event->damage = static_cast<int>(m_UpgradeService.GetDamagePerTick());
            event->hp = static_cast<int>(node->GetHP());
            Notify(event);

            m_HealthService.Reduce(healthCost);
            };

        // Note: We cast m_Nodes to vector<Node*> inside the service? 
        // Ideally the service should accept vector<INode*>, but for now we rely on the implementation.
        // Since we changed m_Nodes to INode*, we might need a cast if the service signature strictly demands Node*.
        // Assuming you updated IDamageZoneService to take vector<INode*> or cast appropriately.

        // For this code to compile with your *existing* IDamageZoneService (which takes vector<Node*>), 
        // we'd have to cast back. But since we are refactoring, we assume the Service now takes INode* or we do a transform.
        // To keep it simple for this file:
        std::vector<Node*> castedNodes;
        for (auto* n : m_Nodes) castedNodes.push_back(dynamic_cast<Node*>(n));

        m_DamageZoneService.ProcessDamageZone(
            m_MouseX,
            m_MouseY,
            m_UpgradeService.GetDamageZoneSize(),
            m_UpgradeService.GetDamagePerTick(),
            m_LevelService.GetCurrentLevel(),
            castedNodes,
            onNodeDamaged);
    }
}

void Game::UpdateNodes(float deltaTime) {
    for (INode* node : m_Nodes) {
        node->Update(deltaTime);
    }

    // Remove Dead Nodes
    m_Nodes.erase(
        std::remove_if(m_Nodes.begin(), m_Nodes.end(),
            [this](INode* node) {
                bool isBoss = node->GetShape() == NodeShape::Boss;

                // Logic: Remove if Dead OR Offscreen (unless it's the boss)
                bool isOffScreen = node->GetPosition().x < BOSS_OFFSCREEN_LIMIT;
                if (isBoss) isOffScreen = false;

                bool shouldRemove = node->GetState() == NodeState::Dead || isOffScreen;

                if (shouldRemove) {
                    if (node->GetState() == NodeState::Dead) {
                        if (isBoss) {
                            // Boss Defeat Logic
                            int pointsGained = POINTS_BOSS * m_LevelService.GetCurrentLevel();
                            auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::BossDefeated);
                            event->level = m_LevelService.GetCurrentLevel();
                            event->points = pointsGained;
                            Notify(event);

                            m_LevelService.SetBossActive(false);
                            m_Boss = nullptr;
                            m_LevelService.SetLevelCompleted(true);
                        }
                        else {
                            // Normal Enemy Defeat Logic
                            auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeDestroyed);
                            event->shape = node->GetShape();
                            event->position = node->GetPosition();
                            event->points = POINTS_NODE;
                            Notify(event);

                            m_PickupService.SpawnPointPickups(node->GetPosition());
                            m_NodesDestroyed++;
                            m_LevelService.IncrementNodesDestroyed();
                        }
                    }
                    delete node;
                }
                return shouldRemove;
            }),
        m_Nodes.end());
}

// -----------------------------------------------------------------------------
// Getters / Setters
// -----------------------------------------------------------------------------

float Game::GetScreenWidth() const { return m_ScreenWidth; }
float Game::GetScreenHeight() const { return m_ScreenHeight; }

// Safe implementation now
const std::vector<INode*>& Game::GetNodes() const {
    return m_Nodes;
}

void Game::SetMousePosition(float x, float y) {
    m_MouseX = x;
    m_MouseY = y;
}

// -----------------------------------------------------------------------------
// Entity Creation
// -----------------------------------------------------------------------------

void Game::SpawnNode(const SpawnInfo& info) {
    float nodeSize = m_ScreenHeight * 0.0375f;
    INode* node = CreateNode(info.shape, nodeSize, GameConfig::NODE_DEFAULT_SPEED);

    float baseHP = node->GetHP();
    float scaledHP = m_SpawnService.CalculateNodeHP(baseHP);
    node->SetHP(scaledHP); // This works now because SetHP is in INode

    node->Spawn(info.position.x, info.position.y);
    node->SetDirection(info.directionX, info.directionY);
    m_Nodes.push_back(node);

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeSpawned);
    event->shape = node->GetShape();
    event->position = info.position;
    event->size = node->GetSize();
    event->hp = static_cast<int>(node->GetHP());
    Notify(event);
}

void Game::SpawnBoss() {
    if (m_LevelService.IsBossActive()) return;

    float bossSize = m_ScreenHeight * 0.15f;
    float bossHP = GameConfig::BOSS_HP_BASE + (m_LevelService.GetCurrentLevel() - 1) * 100.0f;
    m_Boss = CreateNode(NodeShape::Boss, bossSize, GameConfig::BOSS_SPEED);

    m_Boss->SetHP(bossHP);

    // Random edge spawning logic
    float spawnX, spawnY;
    int edge = std::rand() % 4;
    float offset = bossSize * 1.5f;

    switch (edge) {
    case 0: spawnX = (float)rand() / RAND_MAX * m_ScreenWidth; spawnY = -offset; break;
    case 1: spawnX = m_ScreenWidth + offset; spawnY = (float)rand() / RAND_MAX * m_ScreenHeight; break;
    case 2: spawnX = (float)rand() / RAND_MAX * m_ScreenWidth; spawnY = m_ScreenHeight + offset; break;
    case 3: spawnX = -offset; spawnY = (float)rand() / RAND_MAX * m_ScreenHeight; break;
    }

    m_Boss->Spawn(spawnX, spawnY);

    // Calc Direction to Center
    float centerX = m_ScreenWidth / 2.0f;
    float centerY = m_ScreenHeight / 2.0f;
    float dirX = centerX - spawnX;
    float dirY = centerY - spawnY;

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0) { dirX /= length; dirY /= length; }

    m_Boss->SetDirection(dirX, dirY);
    m_Nodes.push_back(m_Boss);
    m_LevelService.SetBossActive(true);

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::BossSpawned);
    event->level = m_LevelService.GetCurrentLevel();
    event->bossHP = bossHP;
    Notify(event);
}

INode* Game::CreateNode(NodeShape shape, float size, float speed) {
    return new Node(shape, size, speed);
}

// -----------------------------------------------------------------------------
// State Management
// -----------------------------------------------------------------------------

void Game::Reset() {
    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    m_ElapsedTime = 0.0f;
    m_PickupService.Reset();
    m_HealthService.Reset(m_UpgradeService.GetMaxHealth());
    m_NodesDestroyed = 0;
    m_SpawnService.ResetSpawnTimer();
    m_DamageZoneService.ResetTimer();

    SaveData saveData = m_SaveService.LoadProgress();
    m_LevelService.Reset(saveData.currentLevel);

    m_Boss = nullptr;
}

void Game::StartNextLevel() {
    int oldLevel = m_LevelService.GetCurrentLevel();
    m_LevelService.StartNextLevel();

    SaveProgress();

    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    m_PickupService.Reset();
    m_SpawnService.ResetSpawnTimer();
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());
    m_HealthService.RestoreToMax();
    m_Boss = nullptr;

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::LevelCompleted);
    event->level = oldLevel;
    event->nextLevel = m_LevelService.GetCurrentLevel();
    Notify(event);
}

void Game::SaveProgress() {
    SaveData saveData = m_SaveService.LoadProgress();

    saveData.totalNodesDestroyed += m_NodesDestroyed;
    saveData.points += m_PickupService.GetPickupPoints();

    if (m_PickupService.GetPickupPoints() > saveData.highPoints) {
        saveData.highPoints = m_PickupService.GetPickupPoints();
        m_HighPoints = m_PickupService.GetPickupPoints();
    }

    saveData.currentLevel = m_LevelService.GetCurrentLevel();
    saveData.maxHealth = m_UpgradeService.GetMaxHealth();
    saveData.regenRate = m_UpgradeService.GetRegenRate();
    saveData.damageZoneSize = m_UpgradeService.GetDamageZoneSize();
    saveData.damagePerTick = m_UpgradeService.GetDamagePerTick();

    m_SaveService.SaveProgress(saveData);
}

int Game::GetNodesDestroyed() const { return m_NodesDestroyed; }
int Game::GetHighPoints() const { return m_HighPoints; }
std::vector<PointPickup> Game::GetCollectedPickupsThisFrame() const { return m_CollectedPickupsThisFrame; }

IUpgradeService& Game::GetUpgradeService() { return m_UpgradeService; }
IPickupService& Game::GetPickupService() { return m_PickupService; }
IHealthService& Game::GetHealthService() { return m_HealthService; }
ILevelService& Game::GetLevelService() { return m_LevelService; }
IDamageZoneService& Game::GetDamageZoneService() { return m_DamageZoneService; }
ISpawnService& Game::GetSpawnService() { return m_SpawnService; }
ISaveService& Game::GetSaveService() { return m_SaveService; }

void Game::Attach(std::shared_ptr<IObserver> observer) { m_Subject.Attach(observer); }
void Game::Detach(std::shared_ptr<IObserver> observer) { m_Subject.Detach(observer); }
void Game::Notify(const std::shared_ptr<IEvent>& event) { m_Subject.Notify(event); }