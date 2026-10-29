#pragma once
#include <cstdint>

namespace impala {

    struct EntityHandle {
        uint32_t id = 0;
        uint32_t generation = 0;
    };

} // namespace impala