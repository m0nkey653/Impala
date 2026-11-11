#include "dzone_player.h"
#include "components.h"
#include "impala/math.h"
#include "impala/transform.h"

namespace dzone {
    void DzonePlayer::Update(float dt, impala::IScene* scene) {
        auto controls = scene->GetComponent<ShipControls>(_entity);
        if (controls) {
            controls->turn = _input.GetRotation();
            controls->throttle = _input.GetThrust();
            controls->firePrimary = _input.GetFirePrimary();
            controls->fireSecondary = _input.GetFireSecondary();
        }
    }

    void DzonePlayer::SetPosition(impala::math::Vec2 position, impala::IScene* scene) {
        auto transform = scene->GetComponent<impala::Transform2d>(_entity);
        if (transform) {
            transform->position = position;
        }
    }

    void DzonePlayer::SetRotation(float rotation, impala::IScene* scene) {
        auto transform = scene->GetComponent<impala::Transform2d>(_entity);
        if (transform) {
            transform->rotation = rotation;
        }
    }

} // namespace dzone
