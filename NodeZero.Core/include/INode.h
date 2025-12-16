#pragma once

#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "Types/Position.h"

/**
 * @class INode
 * @brief Interface for all dynamic entities (enemies) in the game world.
 *
 * Defines the contract for movement, rendering properties (shape, size),
 * and health management.
 */
class INode {
public:
    virtual ~INode() = default;

    // --- State Accessors ---
    virtual Position& GetPosition() = 0;
    virtual const Position& GetPosition() const = 0;
    virtual NodeShape GetShape() const = 0;
    virtual NodeState GetState() const = 0;

    // --- Physical Properties ---
    virtual float GetSize() const = 0;
    virtual float GetSpeed() const = 0;
    virtual float GetRotation() const = 0;

    // --- Health System ---
    virtual float GetHP() const = 0;
    virtual float GetMaxHP() const = 0;

    /**
     * @brief Sets the exact health value (used for initialization or scaling).
     */
    virtual void SetHP(float hp) = 0;

    /**
     * @brief Applies damage to the node.
     * @param damage Amount of health to subtract.
     */
    virtual void TakeDamage(float damage) = 0;

    /**
     * @brief Instantly marks the node as Dead.
     */
    virtual void Kill() = 0;

    // --- Logic ---

    /**
     * @brief Activates the node at a specific world coordinate.
     */
    virtual void Spawn(float x, float y) = 0;

    /**
     * @brief Sets the movement vector.
     */
    virtual void SetDirection(float dirX, float dirY) = 0;

    /**
     * @brief Updates position and rotation based on time delta.
     */
    virtual void Update(float deltaTime) = 0;
};