#pragma once

#include "dzone_player.h"
#include "impala/game.h"

namespace dzone {

    static constexpr int MAX_PLAYER_COUNT = 8;

    class PlayerCollection {
    public:
        DzonePlayer& operator[](int index) {
            if (index < 0 || index >= MAX_PLAYER_COUNT) {
                throw std::out_of_range("Player index out of range " + std::to_string(index));
            }
            return _players[index];
        }

        const uint32_t PlayerCount() const { return static_cast<uint32_t>(_players.size()); }

        DzonePlayer& AddPlayer(std::string name, impala::GameContext* gameContext,
                               impala::IScene* scene, const impala::SpriteHandle sprite);

        void AddGamepadToPlayer(impala::GamepadHandle handle);
        void RemoveGamepadFromPlayer(impala::GamepadHandle handle);

    private:
        std::vector<DzonePlayer> _players;
        std::unordered_map<impala::GamepadHandle, uint32_t> _gamepadToPlayerMap;
    };

} // namespace dzone
