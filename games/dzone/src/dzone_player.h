#pragma once
#include "impala/handles.h"
#include "impala/scene.h"
#include "player_input.h"

namespace dzone {
    class DzonePlayer {
    public:
        DzonePlayer(impala::EntityHandle entity, dzone::PlayerInput input)
            : _entity(entity), _input(input) {}
        void Update(float dt, impala::IScene* scene);

        impala::GamepadHandle GetGamepad() const { return _input.GetGamepadHandle(); }
        impala::EntityHandle GetEntity() const { return _entity; }
        void SetEntity(impala::EntityHandle entity) { _entity = entity; }
        void SetGamepad(impala::GamepadHandle handle) { _input.SetGamepadHandle(handle); }

    private:
        float _rotationSpeed = 60.0f;
        float _thrustSpeed = 60.0f;
        impala::EntityHandle _entity;
        dzone::PlayerInput _input;
    };

} // namespace dzone
