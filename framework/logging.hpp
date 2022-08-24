#ifndef VULKANIMGUITEMPLATE_LOGGING_HPP
#define VULKANIMGUITEMPLATE_LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

namespace logging
{
    constexpr char debug_log_pattern[] = "%^[%d/%m/%Y %T.%e][%l][thread %t] %v%$";
    constexpr char log_pattern[] = "%^[%d/%m/%Y %T.%e][%l] %v%$";

    inline void init(spdlog::level::level_enum level)
    {
        auto log = spdlog::stdout_color_mt("APP");
        spdlog::set_default_logger(log);
        spdlog::set_pattern(level == spdlog::level::debug ? debug_log_pattern : log_pattern);
        spdlog::set_level(level);
        spdlog::flush_on(level);
    }

    template<typename FormatString, typename... Args>
    inline void error(const FormatString& fmt, Args&&...args)
    {
        spdlog::error(fmt, args...);
    }

    template<typename FormatString, typename... Args>
    inline void warn(const FormatString& fmt, Args&&...args)
    {
        spdlog::warn(fmt, args...);
    }

    template<typename FormatString, typename... Args>
    inline void critical(const FormatString& fmt, Args&&...args)
    {
        spdlog::critical(fmt, args...);
    }

    template<typename FormatString, typename... Args>
    inline void info(const FormatString& fmt, Args&&...args)
    {
        spdlog::info(fmt, args...);
    }

    template<typename FormatString, typename... Args>
    inline void debug(const FormatString& fmt, Args&&...args)
    {
        spdlog::debug(fmt, args...);
    }
}

#endif //VULKANIMGUITEMPLATE_LOGGING_HPP