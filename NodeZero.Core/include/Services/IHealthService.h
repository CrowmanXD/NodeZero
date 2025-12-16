#pragma once

/**
 * @class IHealthService
 * @brief Interface defining the contract for health management.
 *
 * This service handles health points (HP), regeneration, damage reduction,
 * and passive health depletion over time (e.g., hunger/decay mechanics).
 */
class IHealthService {
   public:
    virtual ~IHealthService() = default;

    /**
     * @brief Initializes the health service with starting values.
     * @param maxHealth The maximum health value.
     * @param regenRate The amount of health restored per second.
     */
    virtual void Initialize(float maxHealth, float regenRate) = 0;

    /**
     * @brief Updates the health logic (regeneration and depletion).
     * @param deltaTime Time elapsed since the last frame (in seconds).
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Reduces current health by a specific amount.
     * @param amount The amount of damage to take.
     */
    virtual void Reduce(float amount) = 0;

    /**
     * @brief Instantly restores health to the maximum value.
     */
    virtual void RestoreToMax() = 0;

    /**
     * @brief Sets a new maximum health value.
     * @param maxHealth The new maximum health.
     */
    virtual void SetMaxHealth(float maxHealth) = 0;

    /**
     * @brief Sets a new regeneration rate.
     * @param regenRate The new regeneration rate per second.
     */
    virtual void SetRegenRate(float regenRate) = 0;

    /**
     * @brief Gets the current health value.
     * @return The current health.
     */
    virtual float GetCurrent() const = 0;

    /**
     * @brief Gets the maximum health value.
     * @return The maximum health.
     */
    virtual float GetMax() const = 0;

    /**
     * @brief Checks if the entity is dead (health <= 0).
     * @return True if health is zero or less, False otherwise.
     */
    virtual bool IsZero() const = 0;
};
