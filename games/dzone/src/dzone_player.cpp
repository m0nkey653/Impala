#include "dzone_player.h"
#include "impala/math.h"
#include "impala/transform.h"

namespace dzone {
    void DzonePlayer::Update(float dt, impala::IScene* scene) {
        auto rotation = _input.GetRotation();
        auto thrust = _input.GetThrust();

        auto transform = scene->GetComponent<impala::Transform2d>(_entity);

        if (transform) {
            transform->rotation += rotation * _rotationSpeed * dt;
            auto dist = thrust * _thrustSpeed * dt;
            // kenney ships point up
            // y points down, so negative y moves up on the screen
            impala::math::Vec2 velocity = {0.0f, -1 * dist};
            transform->position += impala::math::RotateDegrees(velocity, transform->rotation);
        }
    }

} // namespace dzone
