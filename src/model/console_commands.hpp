/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <string>
#include <vector>

namespace common
{
    class SystemInterface;
}

namespace model
{
    class ConsoleCommands
    {
      public:
        /**
         * @brief Constructs a ConsoleCommands object.
         *
         * @param system_interface The system interface.
         */
        explicit ConsoleCommands(common::SystemInterface* system_interface);

        /**
         * @brief Searches for command matches for the given command.
         *
         * Returns a sorted vector of commands that match the given command.
         *
         * @param command The command to search for matches.
         *
         * @return A sorted vector of matching commands.
         */
        [[nodiscard]] std::vector<std::string> find_command_matches(const std::string& command) const noexcept;

      private:
        /// The system interface.
        common::SystemInterface* system_interface_;
    };
}
