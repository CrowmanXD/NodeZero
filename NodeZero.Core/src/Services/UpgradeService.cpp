#include "UpgradeService.h"

#include "Config/GameConfig.h"
#include "Services/ISaveService.h"
#include "Types/SaveData.h"

#include "Services/Upgrades/HealthUpgradeStrategy.h"
#include "Services/Upgrades/RegenUpgradeStrategy.h"
#include "Services/Upgrades/DamageZoneUpgradeStrategy.h"
#include "Services/Upgrades/DamageUpgradeStrategy.h"

/**
 * @brief Default constructor initializing stats with game configuration defaults.
 */
UpgradeService::UpgradeService()
    : m_MaxHealth(GameConfig::HEALTH_DEFAULT),
    m_RegenRate(0.0f),
    m_DamageZoneSize(GameConfig::DAMAGE_ZONE_DEFAULT_SIZE),
    m_DamagePerTick(GameConfig::DAMAGE_PER_TICK_DEFAULT),
    m_SaveService(nullptr) {
}

/**
 * @brief Initializes the service with values typically loaded from persistent storage.
 */
void UpgradeService::Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick) {
    m_MaxHealth = maxHealth;
    m_RegenRate = regenRate;
    m_DamageZoneSize = damageZoneSize;
    m_DamagePerTick = damagePerTick;
}

/**
 * @brief Injects the dependency for the save system.
 */
void UpgradeService::SetSaveService(ISaveService* saveService) {
    m_SaveService = saveService;
}

/**
 * @brief Executes a generic purchase transaction.
 * * This method coordinates with the SaveService to validate points, apply the
 * specific upgrade logic defined in the strategy, and persist changes.
 */
bool UpgradeService::AttemptPurchase(UpgradeStrategy& strategy) {
    if (!m_SaveService) return false;

    SaveData saveData = m_SaveService->GetCurrentData();

    if (saveData.points < strategy.GetCost() || !strategy.CanApply()) {
        return false;
    }

    saveData.points -= strategy.GetCost();
    strategy.Apply(saveData);

    m_MaxHealth = saveData.maxHealth;
    m_RegenRate = saveData.regenRate;
    m_DamageZoneSize = saveData.damageZoneSize;
    m_DamagePerTick = saveData.damagePerTick;

    m_SaveService->SaveProgress(saveData);
    return true;
}

bool UpgradeService::BuyHealthUpgrade() {
    HealthUpgradeStrategy strategy;
    return AttemptPurchase(strategy);
}

bool UpgradeService::BuyRegenUpgrade() {
    RegenUpgradeStrategy strategy;
    return AttemptPurchase(strategy);
}

bool UpgradeService::BuyDamageZoneUpgrade() {
    DamageZoneUpgradeStrategy strategy;
    strategy.SetCurrentSize(m_DamageZoneSize);
    return AttemptPurchase(strategy);
}

bool UpgradeService::BuyDamageUpgrade() {
    DamageUpgradeStrategy strategy;
    return AttemptPurchase(strategy);
}

int UpgradeService::GetHealthUpgradeCost() const {
    return GameConfig::HEALTH_UPGRADE_COST;
}

float UpgradeService::GetMaxHealth() const {
    return m_MaxHealth;
}

int UpgradeService::GetRegenUpgradeCost() const {
    return GameConfig::REGEN_UPGRADE_COST;
}

float UpgradeService::GetRegenRate() const {
    return m_RegenRate;
}

int UpgradeService::GetDamageZoneUpgradeCost() const {
    return GameConfig::DAMAGE_ZONE_UPGRADE_COST;
}

float UpgradeService::GetDamageZoneSize() const {
    return m_DamageZoneSize;
}

int UpgradeService::GetDamageUpgradeCost() const {
    return GameConfig::DAMAGE_UPGRADE_COST;
}

float UpgradeService::GetDamagePerTick() const {
    return m_DamagePerTick;
}