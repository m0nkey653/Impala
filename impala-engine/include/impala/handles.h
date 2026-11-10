#pragma once

#include <compare> // IWYU pragma: keep  (needed by defaulted operator<=>)
#include <cstdint>
#include <format> // IWYU pragma: keep (needed for std::formatter)
#include <functional>

namespace impala {
    template <typename Tag> struct Handle {
        uint32_t id = UINT32_MAX;
        uint32_t generation = UINT32_MAX;

        constexpr bool IsValid() const { return id != UINT32_MAX; }
        constexpr auto operator<=>(const Handle&) const = default;
        static constexpr Handle Invalid() { return {}; }
    };

    using EntityHandle = Handle<struct EntityTag>;
    using TextureHandle = Handle<struct TextureTag>;
    using SpriteHandle = Handle<struct SpriteTag>;
    using GamepadHandle = Handle<struct GamepadTag>;

} // namespace impala

// keep out of impala namespace for std::hash specialization
template <typename Tag> struct std::hash<impala::Handle<Tag>> {
    size_t operator()(const impala::Handle<Tag>& h) const noexcept {
        return std::hash<uint64_t>{}((uint64_t(h.generation) << 32) | h.id);
    }
};

template <typename Tag> struct std::formatter<impala::Handle<Tag>> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    // Formats the Point object into the output context
    auto format(const impala::Handle<Tag>& h, std::format_context& ctx) const {
        // You can use std::format_to to send the output directly to the buffer
        return std::format_to(ctx.out(), "Handle({}, {})", h.id, h.generation);
    }
};
