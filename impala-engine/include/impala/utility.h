#pragma once

namespace impala::utility {

    template <class... Ts> struct Overloaded : Ts... {
        using Ts::operator()...;
    };

} // namespace impala::utility
