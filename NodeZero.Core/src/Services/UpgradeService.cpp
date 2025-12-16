#include "UpgradeService.h"

#include "Config/GameConfig.h"
#include "Services/ISaveService.h"
#include "Types/SaveData.h"

UpgradeService::UpgradeService()
    : m_MaxHealth(GameConfig::HEALTH_DEFAULT),
    m_RegenRate(0.0f),
    m_DamageZoneSize(GameConfig::DAMAGE_ZONE_DEFAULT_SIZE),
    m_DamagePerTick(GameConfig::DAMAGE_PER_TICK_DEFAULT),
    m_SaveService(nullptr) {
}

void UpgradeService::Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick) {
    m_MaxHealth = maxHealth;
    m_RegenRate = regenRate;
    m_DamageZoneSize = damageZoneSize;
    m_DamagePerTick = damagePerTick;
}

void UpgradeService::SetSaveService(ISaveService* saveService) {
    m_SaveService = saveService;
}

// -----------------------------------------------------------------------------
// Refactor: Upgrade logic now uses AttemptPurchase
// -----------------------------------------------------------------------------

bool UpgradeService::BuyHealthUpgrade() {
    return AttemptPurchase(GameConfig::HEALTH_UPGRADE_COST, [this](SaveData& data) {
        m_MaxHealth += HEALTH_INCREMENT;

        data.maxHealth = m_MaxHealth;
        });
}

bool UpgradeService::BuyRegenUpgrade() {
    return AttemptPurchase(GameConfig::REGEN_UPGRADE_COST, [this](SaveData& data) {
        m_RegenRate += GameConfig::REGEN_UPGRADE_AMOUNT;
        data.regenRate = m_RegenRate;
        });
}

bool UpgradeService::BuyDamageZoneUpgrade() {
    if (m_DamageZoneSize >= GameConfig::DAMAGE_ZONE_MAX_SIZE) {
        return false;
    }

    return AttemptPurchase(GameConfig::DAMAGE_ZONE_UPGRADE_COST, [this](SaveData& data) {
        m_DamageZoneSize += GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT;

        if (m_DamageZoneSize > GameConfig::DAMAGE_ZONE_MAX_SIZE) {
            m_DamageZoneSize = GameConfig::DAMAGE_ZONE_MAX_SIZE;
        }

        data.damageZoneSize = m_DamageZoneSize;
        });
}

bool UpgradeService::BuyDamageUpgrade() {
    return AttemptPurchase(GameConfig::DAMAGE_UPGRADE_COST, [this](SaveData& data) {
        m_DamagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
        data.damagePerTick = m_DamagePerTick;
        });
}

// -----------------------------------------------------------------------------
// Refactor: Common Transaction Handler
// -----------------------------------------------------------------------------

bool UpgradeService::AttemptPurchase(int cost, const std::function<void(SaveData&)>& applyEffect) {
    if (!m_SaveService) {
        return false;
    }

    SaveData saveData = m_SaveService->GetCurrentData();

    if (saveData.points < cost) {
        return false;
    }

    saveData.points -= cost;
    applyEffect(saveData);

    m_SaveService->SaveProgress(saveData);
    return true;
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