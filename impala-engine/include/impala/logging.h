#pragma once
#include <format>
#include <string_view>

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

    namespace detail {
        void VLog(LogCategory category, LogLevel level, std::string_view format,
                  std::format_args args);
    } // namespace detail

    template <typename... Args>
    void Verbose(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Verbose, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Trace(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Trace, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Debug(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Debug, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Info(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Info, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Warn(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Warn, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Error(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Error, format.get(), std::make_format_args(args...));
    }

    template <typename... Args>
    void Critical(LogCategory category, std::format_string<Args...> format, Args&&... args) {
        detail::VLog(category, LogLevel::Critical, format.get(), std::make_format_args(args...));
    }

} // namespace impala::log