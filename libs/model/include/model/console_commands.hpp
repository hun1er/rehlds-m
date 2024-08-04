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

#include <string>
#include <vector>

namespace Common {
    class SystemInterface;
}

namespace Model {
    class ConsoleCommands {
      public:
        /**
         * @brief Constructs a ConsoleCommands object.
         *
         * @param system_interface The system interface.
         */
        explicit ConsoleCommands(Common::SystemInterface* system_interface);

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
        Common::SystemInterface* system_interface_;
    };
}
