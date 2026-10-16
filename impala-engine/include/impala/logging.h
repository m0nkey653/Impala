#pragma once

namespace impala::log {
    enum class LogCategory : int {
        Engine,
        Assets,
        Scene,
        Game,
    };

    enum class LogLevel : int {
        Verbose,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical,
    };
} // namespace impala::log