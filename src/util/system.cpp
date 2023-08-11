/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/system.hpp"
#include <filesystem>
#include <string>
#include <string_view>

namespace util
{
    std::string get_module_canonical_path(const std::string_view filename)
    {
        std::filesystem::path path{filename};
        path.make_preferred();

        if (!path.is_absolute()) {
            auto current_dir = std::filesystem::current_path();
            path = (current_dir /= path);
        }

        return exists(path) ? canonical(path).string() : std::string{};
    }
}
