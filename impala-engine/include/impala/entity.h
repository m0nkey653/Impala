#pragma once

namespace impala {

    struct EntityHandle {
        uint32_t id;
        uint32_t generation;
    };

} // namespace impala