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

#include "model/console_commands.hpp"
#include "common/engine/interface/system_interface.hpp"
#include "common/object_list.hpp"
#include "util/string.hpp"
#include <algorithm>

namespace Model {
    ConsoleCommands::ConsoleCommands(Common::SystemInterface* const system_interface) :
      system_interface_(system_interface)
    {
    }

    std::vector<std::string> ConsoleCommands::find_command_matches(const std::string& command) const noexcept
    {
        std::vector<std::string> matches{};

        if ((nullptr == system_interface_) || command.empty()) {
            return matches;
        }

        Common::ObjectList object_list{};
        system_interface_->get_command_matches(command.c_str(), &object_list);

        for (const auto* const object : object_list) {
            const auto* const command_match = static_cast<const char*>(object);

            if ((nullptr == command_match) || ('\0' == *command_match)) {
                continue;
            }

            if (const auto& command_string = Util::str::trim(command_match); !command_string.empty()) {
                matches.emplace_back(Util::str::to_lower(command_string));
            }
        }

        // Sort ascending
        std::sort(matches.begin(), matches.end());

        return matches;
    }
}
