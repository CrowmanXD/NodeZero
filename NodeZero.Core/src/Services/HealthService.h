#pragma once

#include "Services/IHealthService.h"

/**
 * @class HealthService
 * @brief Concrete implementation of the health system.
 *
 * Implements logic for taking damage, regenerating health over time,
 * and handling passive health depletion based on game levels.
 */
class HealthService : public IHealthService {
   private:
    float m_MaxHealth;
    float m_CurrentHealth;
    float m_RegenRate;
    float m_HealthDepletionRate;
    float m_HealthDepletionInterval;
    float m_HealthTimer;
    int m_CurrentLevel;

    //Refactor: No magic numbers, turned into constants
    static constexpr float BASE_DEPLETION_RATE = 0.1f;
    static constexpr float DEPLETION_INTERVAL = 0.3f;
    static constexpr float LEVEL_SCALING_FACTOR = 0.20f;

   public:
    HealthService();
    ~HealthService() override = default;

    void Initialize(float maxHealth, float regenRate) override;
    void Update(float deltaTime) override;

    /**
     * @brief Resets the service to a specific max health state.
     * @param maxHealth The new max health to set.
     */
    void Reset(float maxHealth);

    void Reduce(float amount) override;
    void SetMaxHealth(float maxHealth);
    void SetRegenRate(float regenRate);

    /**
     * @brief Sets the current game level, which scales the passive depletion rate.
     * @param level The current level index.
     */
    void SetCurrentLevel(int level);
    void RestoreToMax() override;

    float GetCurrent() const override;
    float GetMax() const override;
    bool IsZero() const override;

   private:
       /**
     * @brief Handles the passive regeneration logic.
     */
    void ApplyRegeneration(float deltaTime);

    /**
     * @brief Handles the passive health decay logic based on level scaling.
     */
    void ApplyDepletion(float deltaTime);
};
