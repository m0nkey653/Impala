#pragma once
#include "impala/handles.h"
#include "player_input.h"

namespace dzone {
    class DzonePlayer {
    public:
        DzonePlayer(impala::EntityHandle entity) : _entity(entity) {}
        void Update(float dt);

        impala::GamepadHandle GetGamepad() const { return _input.GetGamepadHandle(); }
        impala::EntityHandle GetEntity() const { return _entity; }
        void SetEntity(impala::EntityHandle entity) { _entity = entity; }
        void SetGamepad(impala::GamepadHandle handle) { _input.SetGamepadHandle(handle); }

    private:
        impala::EntityHandle _entity;
        dzone::PlayerInput _input;
    };

} // namespace dzone
