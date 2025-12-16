#pragma once

/**
 * @enum NodeState
 * @brief Represents the lifecycle state of a game entity (Node).
 */
enum class NodeState {
    /** @brief The node is in the object pool but not currently active in the world. */
    Inactive,

    /** @brief The node is alive, updating, and rendering. */
    Active,

    /** @brief The node has been destroyed (health <= 0) and is awaiting cleanup. */
    Dead
};