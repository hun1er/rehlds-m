/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "util/string.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace model
{
    /**
     * @brief Provides an interface for managing command line arguments.
     *
     * Allows for the construction of an object from command line arguments
     * and retrieving the current command line arguments as a C-style string.
     */
    class CmdLineArgs
    {
      public:
        /**
         * @brief Constructs a CmdLineArgs object from command line arguments.
         *
         * @param argc The number of command line arguments.
         * @param argv An array of command line arguments.
         */
        CmdLineArgs(int argc, const char* const* argv);

        /**
         * @brief Gets the current command line arguments as a C-style string.
         *
         * @return A pointer to a null-terminated C-style string containing the current command line arguments.
         */
        [[nodiscard]] const char* get_current() const noexcept;

        /**
         * @brief Adds a parameter and its option to the front of the command line arguments.
         *
         * @param param The name of the parameter to add.
         * @param option The option of the parameter to add.
         */
        void push_front(std::string param, std::string option = "");

        /**
         * @brief Adds a parameter and its option to the back of the command line arguments.
         *
         * @param param The name of the parameter to add.
         * @param option The option of the parameter to add.
         */
        void push_back(std::string param, std::string option = "");

        /**
         * @brief Removes a specified parameter and its option from the command line arguments.
         *
         * @param param The name of the parameter to remove.
         */
        void remove(std::string_view param);

        /**
         * @brief Checks if the command line arguments contain a specified parameter.
         *
         * @param param The name of the parameter to check for.
         *
         * @return \c true if the command line arguments contain the specified parameter, \c false otherwise.
         */
        [[nodiscard]] bool contains(std::string_view param) const noexcept;

        /**
         * @brief Finds the index of a specified parameter.
         *
         * @param argc The number of arguments.
         * @param argv An array of arguments.
         * @param param The name of the parameter to find the index of.
         *
         * @return The index of the specified parameter in the arguments, or -1 if the parameter is not found.
         */
        [[nodiscard]] static int find_param_index(int argc, const char* const* argv, std::string_view param) noexcept;

        /**
         * @brief Retrieves the option of a specified parameter as a string.
         *
         * @param argc The number of arguments.
         * @param argv An array of arguments.
         * @param param The name of the parameter whose option is to be retrieved.
         *
         * @return An optional containing the option of the specified parameter as a string.
         *         If the parameter is not found or has no associated option, an empty optional is returned.
         */
        [[nodiscard]] static std::optional<std::string> get_param_option(int argc, const char* const* argv,
                                                                         std::string_view param) noexcept;
        /**
         * @brief Retrieves the option of a specified parameter as a string.
         *
         * @param param The name of the parameter whose option is to be retrieved.
         *
         * @return An optional containing the option of the specified parameter as a string.
         *         If the parameter is not found or has no associated option, an empty optional is returned.
         */
        [[nodiscard]] std::optional<std::string> get_param_option(std::string param) const;

        /**
         * @brief Retrieves the option of a specified parameter as a value of the specified type.
         *
         * @tparam T The type to convert the parameter option to.
         *
         * @param param The name of the parameter whose option is to be retrieved and converted.
         *
         * @return An optional containing the option of the specified parameter
         * converted to the specified type. If the parameter is not found, has no associated option,
         * or cannot be converted to the specified type, an empty optional is returned.
         */
        template <typename T>
        [[nodiscard]] std::optional<T> get_param_option_as(std::string param) const;

      private:
        /// Contains the current command line arguments.
        std::string params_{};

        /// Parses a vector of command line arguments.
        void parse(const std::vector<std::string>& args) noexcept;
    };

    inline const char* CmdLineArgs::get_current() const noexcept
    {
        return params_.c_str();
    }

    template <typename T>
    std::optional<T> CmdLineArgs::get_param_option_as(std::string param) const
    {
        if (const auto& param_option = get_param_option(std::move(param)); param_option) {
            return util::str::convert_to_type<T>(*param_option);
        }

        return std::nullopt;
    }
}
