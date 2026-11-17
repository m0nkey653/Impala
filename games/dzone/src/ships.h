#pragma once
#include "components.h"
#include "dzone_assets.h"
#include "impala/colliders.h"

namespace dzone {

    constexpr int MAX_PLAYER_SHIP_COUNT = 4;

    const char* defaultPlayerShip[MAX_PLAYER_SHIP_COUNT] = {
        KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_BLUE, KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_GREEN,
        KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_RED, KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_ORANGE};

    constexpr std::tuple<std::array<impala::ColliderShape, 8>, int> DefaultPlayerShipColliders() {
        auto shapeHull = impala::ColliderShape{.offset = {.x = 0.0f, .y = -20.0f},
                                               .shape = impala::CircleCollider{.radius = 8.0f}};
        auto nose = impala::ColliderShape{.offset = {.x = 0.0f, .y = 10.0f},
                                          .shape = impala::CircleCollider{.radius = 20.0f}};
        auto leftWing = impala::ColliderShape{.offset = {.x = -28.0f, .y = 12.0f},
                                              .shape = impala::CircleCollider{.radius = 10.0f}};
        auto rightWing = impala::ColliderShape{.offset = {.x = 28.0f, .y = 12.0f},
                                               .shape = impala::CircleCollider{.radius = 10.0f}};
        return {{shapeHull, nose, leftWing, rightWing, {}, {}, {}, {}}, 4};
    }

    struct PlayerShip {
        Thrust thrust = {0, 0, 0, 0, 0};
        Health health = {100};
        Armor armor = {0};
        Shield shield = {0};
        Weapons weapons = {};
        std::array<impala::ColliderShape, 8> colliders;
        int colliderCount = 0;
        const char* spriteName = nullptr;
    };

    constexpr PlayerShip DefaultShip(int index) {
        return PlayerShip{
            .thrust = {.acceleration = 1, .maxSpeed = 100, .drag = 1, .grip = 1, .turnRate = 100},
            .health = {100},
            .armor = {0},
            .shield = {0},
            .weapons = {},
            .colliders = std::get<0>(DefaultPlayerShipColliders()),
            .colliderCount = std::get<1>(DefaultPlayerShipColliders()),
            .spriteName = defaultPlayerShip[index]};
    }

} // namespace dzone
