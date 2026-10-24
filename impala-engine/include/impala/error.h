#pragma once

#include <stdexcept>

namespace impala {
    class ImpalaError : public std::runtime_error {
    public:
        explicit ImpalaError(const std::string& message) : std::runtime_error(message) {}
    };

    class AssetError : public ImpalaError {
    public:
        explicit AssetError(const std::string& message) : ImpalaError(message) {}
    };
} // namespace impala