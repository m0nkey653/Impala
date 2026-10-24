#include "impala/log.h"

namespace impala::log::detail {

    SDL_LogPriority GetSdlLogLevel(LogLevel level) {
        switch (level) {
        case LogLevel::Verbose:
            return SDL_LOG_PRIORITY_VERBOSE;
        case LogLevel::Trace:
            return SDL_LOG_PRIORITY_TRACE;
        case LogLevel::Debug:
            return SDL_LOG_PRIORITY_DEBUG;
        case LogLevel::Info:
            return SDL_LOG_PRIORITY_INFO;
        case LogLevel::Warn:
            return SDL_LOG_PRIORITY_WARN;
        case LogLevel::Error:
            return SDL_LOG_PRIORITY_ERROR;
        case LogLevel::Critical:
            return SDL_LOG_PRIORITY_CRITICAL;
        default:
            return SDL_LOG_PRIORITY_INFO;
        }
    }

    void VLog(LogCategory category, LogLevel level, std::string_view format,
              std::format_args args) {

        const int sdlCategory = SDL_LOG_CATEGORY_CUSTOM + static_cast<int>(category);
        const SDL_LogPriority sdlPriority = GetSdlLogLevel(level); // your switch

        if (sdlPriority < SDL_GetLogPriority(sdlCategory)) {
            return;
        }

        const std::string text = std::vformat(format, args);
        SDL_LogMessage(sdlCategory, sdlPriority, "%s", text.c_str());
    }

} // namespace impala::log::detail