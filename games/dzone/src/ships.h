#pragma once
#include "components.h"

namespace dzone {

    constexpr int MAX_PLAYER_SHIP_COUNT = 4;

    struct PlayerShip {
        Thrust thrust = {0, 0, 0, 0, 0};
        Health health = {100};
        Armor armor = {0};
        Shield shield = {0};
        Weapons weapons = {};
    };

    constexpr PlayerShip DefaultShip() {
        return PlayerShip{
            .thrust = {.acceleration = 1, .maxSpeed = 20, .drag = 1, .grip = 1, .turnRate = 20},
            .health = {100},
            .armor = {0},
            .shield = {0},
            .weapons = {}};
    }

} // namespace dzone
