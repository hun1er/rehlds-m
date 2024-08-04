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

#include "util/string.hpp"
#include <cstddef>
#include <optional>
#include <string>
#include <utility>

namespace Core {
    class CmdLineArgs {
      public:
        [[nodiscard]] static auto from_command_line(int argc, const char* const* argv) -> CmdLineArgs;

        [[nodiscard]] static auto find_argument_index(int argc, const char* const* argv, std::string arg) -> int;

        [[nodiscard]] static auto contains(int argc, const char* const* argv, std::string arg) -> bool;

        /**
         * @brief Checks if the command line arguments contain a specified argument.
         *
         * @param arg The name of the argument to check for.
         *
         * @return \c true if the command line arguments contain the specified argument, \c false otherwise.
         */
        [[nodiscard]] auto contains(std::string arg) const noexcept -> bool;

        /**
         * @brief Adds an argument and its option to the front of the command line arguments.
         *
         * @param arg The name of the argument to add.
         * @param option The option of the argument to add.
         */
        auto push_front(std::string arg, std::string option = "") -> void;

        /**
         * @brief Adds an argument and its option to the back of the command line arguments.
         *
         * @param arg The name of the argument to add.
         * @param option The option of the argument to add.
         */
        auto push_back(std::string arg, std::string option = "") -> void;

        /**
         * @brief Removes a specified argument and its option from the command line arguments.
         *
         * @param arg The name of the argument to remove.
         */
        auto remove(std::string arg) -> void;

        [[nodiscard]] auto find_argument_pos(std::string arg) const -> std::size_t;

        /**
         * @brief Retrieves the option of a specified argument as a string.
         *
         * @param arg The name of the argument whose option is to be retrieved.
         *
         * @return An optional containing the option of the specified argument as a string.
         *         If the argument is not found or has no associated option, an empty optional is returned.
         */
        [[nodiscard]] auto get_argument_option(std::string arg) const -> std::optional<std::string>;

        [[nodiscard]] auto get_argument_option(std::string arg, std::size_t pos) const -> std::optional<std::string>;

        /**
         * @brief Retrieves the option of a specified argument as a value of the specified type.
         *
         * @tparam T The type to convert the argument option to.
         *
         * @param arg The name of the argument whose option is to be retrieved and converted.
         *
         * @return An optional containing the option of the specified argument
         * converted to the specified type. If the argument is not found, has no associated option,
         * or cannot be converted to the specified type, an empty optional is returned.
         */
        template <typename T>
        [[nodiscard]] auto get_argument_option_as(std::string arg) const -> std::optional<T>;

        /**
         * @brief Gets the current command line arguments as a C-style string.
         *
         * @return A pointer to a null-terminated C-style string containing the current command line arguments.
         */
        [[nodiscard]] auto get_current() const noexcept -> const char*;

      private:
        explicit CmdLineArgs(std::string arguments);

        std::string arguments_;
    };

    inline CmdLineArgs::CmdLineArgs(std::string arguments) : arguments_(std::move(arguments))
    {
    }

    inline auto CmdLineArgs::contains(std::string arg) const noexcept -> bool
    {
        return find_argument_pos(arg) != std::string::npos;
    }

    template <typename T>
    std::optional<T> CmdLineArgs::get_argument_option_as(std::string arg) const
    {
        if (const auto& option = get_argument_option(std::move(arg)); option) {
            return Util::str::convert_to_type<T>(*option);
        }

        return std::nullopt;
    }

    inline auto CmdLineArgs::get_current() const noexcept -> const char*
    {
        return arguments_.c_str();
    }
}
