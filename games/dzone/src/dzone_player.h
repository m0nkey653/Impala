#pragma once
#include "impala/handles.h"

namespace dzone {
    class DzonePlayer {
    public:
        DzonePlayer();
        void Update(float dt);
        void Render();

        impala::GamepadHandle GetHandle() const { return _handle; }
        impala::EntityHandle GetEntity() const { return _entity; }
        void SetHandle(impala::GamepadHandle handle) { _handle = handle; }
        void SetEntity(impala::EntityHandle entity) { _entity = entity; }

    private:
        impala::GamepadHandle _handle;
        impala::EntityHandle _entity;
    };

} // namespace dzone
