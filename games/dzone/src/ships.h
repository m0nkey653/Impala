#pragma once
#include "components.h"
#include "dzone_assets.h"

namespace dzone {

    constexpr int MAX_PLAYER_SHIP_COUNT = 4;

    const char* defaultPlayerShip[MAX_PLAYER_SHIP_COUNT] = {
        KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_BLUE, KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_GREEN,
        KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_RED, KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_ORANGE};

    struct PlayerShip {
        Thrust thrust = {0, 0, 0, 0, 0};
        Health health = {100};
        Armor armor = {0};
        Shield shield = {0};
        Weapons weapons = {};
    };

    constexpr PlayerShip DefaultShip() {
        return PlayerShip{
            .thrust = {.acceleration = 1, .maxSpeed = 100, .drag = 1, .grip = 1, .turnRate = 100},
            .health = {100},
            .armor = {0},
            .shield = {0},
            .weapons = {}};
    }

} // namespace dzone
