/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "model/console_commands.hpp"
#include "common/engine/interface/system_interface.hpp"
#include "common/object_list.hpp"
#include "util/string.hpp"
#include <algorithm>

namespace model
{
    ConsoleCommands::ConsoleCommands(common::SystemInterface* const system_interface) :
      system_interface_(system_interface)
    {
    }

    std::vector<std::string> ConsoleCommands::find_command_matches(const std::string& command) const noexcept
    {
        std::vector<std::string> matches{};

        if ((nullptr == system_interface_) || command.empty()) {
            return matches;
        }

        common::ObjectList object_list{};
        system_interface_->get_command_matches(command.c_str(), &object_list);

        for (const auto* const object : object_list) {
            const auto* const command_match = static_cast<const char*>(object);

            if ((nullptr == command_match) || ('\0' == *command_match)) {
                continue;
            }

            if (const auto& command_string = util::str::trim(command_match); !command_string.empty()) {
                matches.emplace_back(util::str::to_lower(command_string));
            }
        }

        // Sort ascending
        std::sort(matches.begin(), matches.end());

        return matches;
    }
}
