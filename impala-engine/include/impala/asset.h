#pragma once

#include <string>

namespace impala {
    class IAsset {
    public:
        virtual ~IAsset() = default;
        virtual std::string Name() = 0;
        virtual void Load() = 0;
    };

} // namespace impala
