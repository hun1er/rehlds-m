/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace abstract
{
    class View;
}

namespace util
{
    /**
     * @brief Logging levels.
     */
    enum class LogLevel : int
    {
        /** @brief Trace level logging. */
        trace = spdlog::level::trace,

        /** @brief Debug level logging. */
        debug = spdlog::level::debug,

        /** @brief Info level logging. */
        info = spdlog::level::info,

        /** @brief Warning level logging. */
        warn = spdlog::level::warn,

        /** @brief Error level logging. */
        error = spdlog::level::err,

        /** @brief Critical level logging. */
        critical = spdlog::level::critical,

        /** @brief Turn off logging. */
        off = spdlog::level::off
    };

    /**
     * @brief LogOutput options for logging.
     */
    enum class LogOutput : unsigned
    {
        /** @brief Log output to view. */
        view = 1U << 0U,

        /** @brief Log output to file. */
        file = 1U << 1U,

        /** @brief Log output to both console and file. */
        view_and_file = view | file
    };

    /**
     * @brief Contains the settings for the logger.
     *
     * Specifies the logging level, the level to flush the logger on, the output options for the logger,
     * and the logfile name for file output.
     */
    struct LogSettings
    {
        /**
         * @brief Logging level.
         *
         * Determines which messages will be logged.
         */
        LogLevel level{LogLevel::info};

        /**
         * @brief LogLevel to flush the logger on.
         *
         * Determines which messages will trigger a flush of the logger's buffer.
         */
        LogLevel flush_on{LogLevel::trace};

        /**
         * @brief LogOutput options for the logger.
         *
         * Determines where the log messages will be written to.
         */
        LogOutput output{LogOutput::view};

        /**
         * @brief Logfile name for file output.
         *
         * Specifies the name of the file to write log messages to when file output is enabled.
         */
        std::string logfile{};

        /**
         * @brief A shared pointer to an abstract View object.
         *
         * Specifies the View object to be used for displaying log messages when view output is enabled.
         */
        std::shared_ptr<abstract::View> view{};
    };

    /**
     * @brief Initializes the logger with the given settings.
     *
     * @param settings The settings to initialize the logger with.
     */
    void log_init(const LogSettings& settings);

    /**
     * @brief Initializes the logger with the given settings.
     *
     * @param con_debug If true, the logger will output to both the console and a file.
     * @param logfile The name of the file to write log messages to when file output is enabled.
     * @param view The View object to be used for displaying log messages when view output is enabled.
     */
    void log_init(bool con_debug, std::string logfile, std::shared_ptr<abstract::View> view);

    /**
     * @brief Returns a constant reference to the \c util::LogSettings object
     * that contains the settings for the logger.
     *
     * @return A constant reference to the LogSettings object.
     */
    [[nodiscard]] const LogSettings& log_settings();

    /**
     * @brief Logs a message with info level.
     *
     * @tparam Args The types of the arguments to be used for formatting the message string.
     *
     * @param str The format string for the message.
     * @param args The arguments to be used for formatting the message string.
     */
    template <typename... Args>
    void log_info(const std::string_view str, Args&&... args)
    {
        spdlog::info(str, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with debug level.
     *
     * @tparam Args The types of the arguments to be used for formatting the message string.
     *
     * @param str The format string for the message.
     * @param args The arguments to be used for formatting the message string.
     */
    template <typename... Args>
    void log_debug(const std::string_view str, Args&&... args)
    {
        spdlog::debug(str, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with warn level.
     *
     * @tparam Args The types of the arguments to be used for formatting the message string.
     *
     * @param str The format string for the message.
     * @param args The arguments to be used for formatting the message string.
     */
    template <typename... Args>
    void log_warn(const std::string_view str, Args&&... args)
    {
        spdlog::warn(str, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with error level.
     *
     * @tparam Args The types of the arguments to be used for formatting the message string.
     *
     * @param str The format string for the message.
     * @param args The arguments to be used for formatting the message string.
     */
    template <typename... Args>
    void log_error(const std::string_view str, Args&&... args)
    {
        spdlog::error(str, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with log_critical level.
     *
     * @tparam Args The types of the arguments to be used for formatting the message string.
     *
     * @param str The format string for the message.
     * @param args The arguments to be used for formatting the message string.
     */
    template <typename... Args>
    void log_critical(const std::string_view str, Args&&... args)
    {
        spdlog::critical(str, std::forward<Args>(args)...);
    }
}

/**
 * @brief Bitwise OR operator for the \c LogOutput enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 *
 * @return The result of the bitwise OR operation.
 */
[[nodiscard]] constexpr util::LogOutput operator|(const util::LogOutput lhs, const util::LogOutput rhs) noexcept
{
    return static_cast<util::LogOutput>(static_cast<std::underlying_type_t<util::LogOutput>>(lhs) |
                                        static_cast<std::underlying_type_t<util::LogOutput>>(rhs));
}

/**
 * @brief Bitwise AND operator for the \c LogOutput enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 *
 * @return The result of the bitwise AND operation.
 */
[[nodiscard]] constexpr util::LogOutput operator&(const util::LogOutput lhs, const util::LogOutput rhs) noexcept
{
    return static_cast<util::LogOutput>(static_cast<std::underlying_type_t<util::LogOutput>>(lhs) &
                                        static_cast<std::underlying_type_t<util::LogOutput>>(rhs));
}
