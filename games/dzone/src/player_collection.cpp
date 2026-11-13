#pragma once

#include "player_collection.h"
#include "components.h"
#include "impala/log.h"
#include "impala/sprite.h"
#include "impala/transform.h"
#include "projectile.h"
#include "ships.h"

namespace dzone {

    DzonePlayer& PlayerCollection::AddPlayer(std::string name, impala::GameContext* gameContext,
                                             impala::IScene* scene,
                                             const impala::SpriteHandle sprite) {
        if (_players.size() >= MAX_PLAYER_COUNT) {
            throw std::out_of_range("Cannot add more players, max count reached");
        }
        auto entity = scene->CreateEntity(name.c_str());
        auto& t = scene->AddComponent<impala::Transform2d>(entity);

        auto ship = DefaultShip();
        scene->AddComponent<Movement>(entity);
        scene->AddComponent<ShipControls>(entity);
        scene->AddComponent<Health>(entity, ship.health);
        scene->AddComponent<Armor>(entity, ship.armor);
        scene->AddComponent<Shield>(entity, ship.shield);
        scene->AddComponent<Thrust>(entity, ship.thrust);
        auto& w = scene->AddComponent<Weapons>(entity, ship.weapons);
        WeaponSlot ws = {.projectile = Laser(), .muzzleOffset = {0.0f, 0.0f}};
        w.slots.push_back(ws);
        w.activePrimary = 0;
        w.activeSecondary = 0;

        scene->AddComponent<impala::SpriteRenderer>(entity);
        auto spriteRenderer = scene->GetComponent<impala::SpriteRenderer>(entity);
        spriteRenderer->sprite = sprite;

        PlayerInput input0(gameContext->keyboard, dzone::KEYBOARD_MAPS[_players.size()],
                           gameContext->gamepads);

        impala::log::Debug(impala::log::LogCategory::Game, "Created Player[{}] {} with entityID {}",
                           _players.size(), name, entity.id);

        _players.push_back(DzonePlayer(entity, input0));
        return _players.back();
    }

    void PlayerCollection::AddGamepadToPlayer(impala::GamepadHandle handle) {
        if (_gamepadToPlayerMap.find(handle) == _gamepadToPlayerMap.end()) {
            for (uint32_t i = 0; i < _players.size(); ++i) {
                if (std::find_if(_gamepadToPlayerMap.begin(), _gamepadToPlayerMap.end(),
                                 [i](const auto& pair) { return pair.second == i; }) ==
                    _gamepadToPlayerMap.end()) {
                    _gamepadToPlayerMap[handle] = i;
                    _players[i].SetGamepad(handle);
                    break;
                }
            }
        }
    }

    void PlayerCollection::RemoveGamepadFromPlayer(impala::GamepadHandle handle) {
        auto it = _gamepadToPlayerMap.find(handle);
        if (it != _gamepadToPlayerMap.end()) {
            _players[it->second].SetGamepad({});
            _gamepadToPlayerMap.erase(it);
        }
    }

} // namespace dzone
