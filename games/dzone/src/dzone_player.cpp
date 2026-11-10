#include "dzone_player.h"
#include "impala/transform.h"

namespace dzone {
    void DzonePlayer::Update(float dt, impala::IScene* scene) {
        auto rotation = _input.GetRotation();
        auto thrust = _input.GetThrust();

        auto transform = scene->GetComponent<impala::Transform2d>(_entity);

        if (transform) {
            transform->rotation += rotation * _rotationSpeed * dt;
            transform->position += {0.0f, thrust * _thrustSpeed * dt};
        }
    }

} // namespace dzone
