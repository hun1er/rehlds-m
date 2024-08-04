/*
 * Half Life 1 SDK License
 * Copyright(c) Valve Corp
 *
 * DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 * DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 * DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 * FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 * LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 * SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * For commercial use, contact: sourceengine@valvesoftware.com
 */

#pragma once

#include "util/log_output.hpp"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Util {
    /**
     * @brief Logging levels.
     */
    enum class LogLevel : int {
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
     * @brief Logging destinations for output.
     *
     * Specifies the different destinations where log messages can be directed.
     */
    enum class LogDestination : unsigned {
        /**
         * @brief Log output to the view (e.g., console or UI).
         *
         * Directs log messages to a visual interface, such as a console window
         * or a graphical user interface (GUI) component.
         */
        view = 1U << 0U,

        /**
         * @brief Log output to a file.
         *
         * Writes log messages to a specified file on the filesystem.
         */
        file = 1U << 1U,

        /**
         * @brief Log output to both a view and a file.
         *
         * Simultaneously directs log messages to both a visual interface (e.g., console or UI) and a file.
         */
        view_and_file = view | file
    };

    /**
     * @brief Contains the settings for the logger.
     *
     * Specifies the logging level, the level to flush the logger on, the output options for the logger,
     * and the logfile name for file output.
     */
    struct LogSettings {
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
         * @brief OutputType options for the logger.
         *
         * Determines where the log messages will be written to.
         */
        LogDestination output{LogDestination::view};

        /**
         * @brief Logfile name for file output.
         *
         * Specifies the name of the file to write log messages to when file output is enabled.
         */
        std::string logfile{};

        /**
         * @brief A shared pointer to the object to be used for displaying log messages when view output is enabled.
         */
        std::shared_ptr<LogOutput> log_output{};
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
     * @param log_output The object to be used for displaying log messages when view output is enabled.
     */
    void log_init(bool con_debug, std::string logfile, std::shared_ptr<LogOutput> log_output);

    /**
     * @brief Returns a constant reference to the \c Util::LogSettings object
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
 * @brief Bitwise OR operator for the \c OutputType enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 *
 * @return The result of the bitwise OR operation.
 */
[[nodiscard]] constexpr Util::LogDestination
  operator|(const Util::LogDestination lhs, const Util::LogDestination rhs) noexcept
{
    return static_cast<Util::LogDestination>(
      static_cast<std::underlying_type_t<Util::LogDestination>>(lhs) |
      static_cast<std::underlying_type_t<Util::LogDestination>>(rhs));
}

/**
 * @brief Bitwise AND operator for the \c OutputType enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 *
 * @return The result of the bitwise AND operation.
 */
[[nodiscard]] constexpr Util::LogDestination
  operator&(const Util::LogDestination lhs, const Util::LogDestination rhs) noexcept
{
    return static_cast<Util::LogDestination>(
      static_cast<std::underlying_type_t<Util::LogDestination>>(lhs) &
      static_cast<std::underlying_type_t<Util::LogDestination>>(rhs));
}
